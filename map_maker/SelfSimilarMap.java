import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

/**
 * SelfSimilarMap
 */
public class SelfSimilarMap {

    public static void main(String[] args) {
        // the length of the area
        double x = Double.parseDouble(args[0]);
        // the length of the area
        double y = Double.parseDouble(args[1]);
        // Hurst parameter (0.5, 1.0)
        double hurst = Double.parseDouble(args[2]);
        // Number of waypoints
        int noOfWaypoints = Integer.parseInt(args[3]);
        // seed
        int seed = Integer.parseInt(args[4]);
        // filename
        String filename = args[5];
        // Random object
        Random rand = new Random(seed);
        // Number of levels as mentioned in original paper "SLAW: A Mobility Model for Human Walks" 
        // in: Proc. of the IEEE Infocom (2009).
        int levels = 8;
        // convert hurst to alpha as done in matlab implementation by Seongik
        // Hong, NCSU, US (3/10/2009)
        double converted_hurst = 2 - 2 * hurst;
        // initial variance at first level as used in matlab implementation by
        // Seongik Hong, NCSU, US (3/10/2009)
        double initial_variance = 0.8;

        // variances for all levels
        double[] xvalues = new double[levels];
        double[] level_variances = new double[levels];

        for (int i = 0; i < levels; i++) {
            xvalues[i] = Math.pow(4, i + 1);
            level_variances[i] = initial_variance * Math.pow(xvalues[i] / xvalues[0], converted_hurst);
        }

        Hashtable<String, Integer> wpoints = new Hashtable<String, Integer>();
        Hashtable<String, Double> Xoffset = new Hashtable<String, Double>();
        Hashtable<String, Double> Yoffset = new Hashtable<String, Double>();
        wpoints.put("0,0", noOfWaypoints);
        Xoffset.put("0,0", 0.0);
        Yoffset.put("0,0", 0.0);
        double Xwind, Ywind;

        for (int level = 0; level < levels; level++) {
            System.out.println("Level " + (level + 1) + " of " + levels + " started.");
            // Number of squares at current level
            double n_squares = Math.pow(4, level);
            Xwind = x / Math.pow(2, level);
            Ywind = y / Math.pow(2, level);

            for (int square = 0; square < n_squares; square++) {
                // if (square % 2000 == 0 && square != 0) {
                //     System.out.println(square + " of " + n_squares + " processed.");
                // }
                // generate the offsets of x and y for children squares
                double val;
                double xval = Xoffset.get(level + "," + square);
                double yval = Yoffset.get(level + "," + square);

                for (int i = 0; i < 4; i++) {
                    val = xval;
                    // add window size to the Xoff set of second and third child square
                    if (i == 1 || i == 3) {
                        val += Xwind / 2;
                    }
                    Xoffset.put((level + 1) + "," + (4 * square + i), val);
                    
                    val = yval;
                    // add window size to the Yoff set of third and fourth child square
                    if (i == 2 || i == 3) {
                        val += Ywind / 2;
                    }
                    Yoffset.put((level + 1) + "," + (4 * square + i), val);
                }

                // get waypoints assigned to this node
                int wp = wpoints.get(level + "," + square);
                if (wp == 0) {
                    // assign 0 to all child nodes as waypoints
                    for (int i = 0; i < 4; i++) {
                        wpoints.put((level + 1) + "," + (4 * square + i), 0);
                    }
                } else if (level == 0) {
                    // first level
                    int[] num = divide_waypoints(wp, level_variances[level], rand);
                    for (int i = 0; i < 4; i++) {
                        wpoints.put((level + 1) + "," + (4 * square + i), num[i]);
                    }
                } else {
                    // inner levels
                    double[] cur_wp = new double[(int)Math.pow(4, level)];
                    for (int i = 0; i < cur_wp.length; i++) {
                        cur_wp[i] = wpoints.get(level + "," + i);
                    }

                    double avg = calculate_average(cur_wp);

                    for (int i = 0; i < Math.pow(4, level); i++) {
                        cur_wp[i] /= avg;
                    }

                    double var = calculate_var(cur_wp) + 1;
                    int[] num = divide_waypoints(wp, ((level_variances[level] + 1) / var) - 1, rand);
                    for (int i = 0; i < 4; i++) {
                        wpoints.put((level + 1) + "," + (4 * square + i), num[i]);
                    }
                }
            }// for squares
        }// for level

        System.out.println("Control point passed\n");

        // create waypoints
        Xwind = x / Math.sqrt(Math.pow(4, levels));
        Ywind = y / Math.sqrt(Math.pow(4, levels));
        int total_squares = (int)Math.pow(4, levels);

        double theta, xx, yy;
        double[] waypointx = new double[noOfWaypoints];
        double[] waypointy = new double[noOfWaypoints];
        int w, iterator = 0;
        for (int i = 0; i < total_squares; i++) {
            // get waypoints of current square
            w = wpoints.get(levels + "," + i);
            if (w != 0) {
                for (int j = 0; j < w; j++) {
                    theta = 2 * Math.PI * rand.nextDouble();
                    xx = Xoffset.get(levels + "," + i) + Xwind / 2 + (rand.nextDouble() * Xwind / 2) * Math.cos(theta);
                    yy = Yoffset.get(levels + "," + i) + Ywind / 2 + (rand.nextDouble() * Ywind / 2) * Math.sin(theta);
                    waypointx[iterator] = xx;
                    waypointy[iterator] = yy;
                    ++iterator;
                }
            }
        }
        try {
            print_file(filename, waypointx, waypointy);
        } catch (Exception e) {
            //TODO: handle exception
            System.out.println(e.toString());
        }
        
    }

    /**
     * Divides the waypoints according to variance and returns four numbers
     * 
     * @param wp
     *            Number of waypoints
     * @param var
     *            variance
     * @return array of divided waypoints of length four
     */
    public static int[] divide_waypoints(int wp, double var, Random rand) {
        double gran = 0.01;
        double Error = 0.03;
        int Thresh = 31;
        int[] num = {0, 0, 0, 0};

        if (var > 4) {
            num[(int)Math.floor(rand.nextDouble() * 4)] = wp;
        }
        else if (var <= 0) {
            int i = 0;
            while (i < wp) {
                num[i % 4] += 1;
                i++;
            }
        }
        else {
            int count = 0;
            double[][] arrays = new double[Thresh][4];

            for (double i = 0; i <= 1; i += gran) {
                for (double j = (1 - i) / 3; j <= 1 - i; j += gran) {
                    for (double k = (1 - i - j) / 2; k <= 1 - i - j; k += gran) {
                        double l = 1 - i - j - k;
                        double[] arr = {i, j, k, l};
                        double avg = calculate_average(arr);
                        double[] arr2 = {arr[0] / avg, arr[1] / avg, arr[2] / avg, arr[3] / avg};

                        if (Math.abs(var - calculate_var(arr2)) < Error * var) {
                            for (int x = 0; x < 4; x++) {
                                arrays[count][x] = arr[x];
                            }

                            if (++count >= Thresh) {
                                break;
                            }
                         }
                    }

                    if (count >= Thresh) {
                        break;
                    }
                }

                if (count >= Thresh) {
                    break;
                }
            }

            // pick a random row
            int row = (int)Math.floor(Thresh * rand.nextDouble());
            double[] rand_arr = {rand.nextDouble(), rand.nextDouble(), rand.nextDouble(), rand.nextDouble()};
            double[] rand_arr2 = rand_arr.clone();
            Arrays.sort(rand_arr2);
            double[] row_rand = new double[4];

            // randomize probability
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    if (rand_arr2[k] == rand_arr[j]) {
                        row_rand[k] = arrays[row][j];
                        break;
                    }
                }
            }

            // distribute waypoints
            double[] prob = {row_rand[3], row_rand[3] + row_rand[2], row_rand[3] + row_rand[2] + row_rand[1],
                    row_rand[0] + row_rand[1] + row_rand[2] + row_rand[3]};
                    
            if (row_rand[0] <= 0 && row_rand[1] <= 0 && row_rand[2] <= 0 && row_rand[3] <= 0) {
            	    System.out.println("Error: Variance too large, try again!");
                    System.exit(0);   
            }
            for (int i = 0; i < wp;) {
                double rand_ = rand.nextDouble();
                for (int j = 0; j < 4; j++) {
                    if (rand_ <= prob[j]) {
                        num[j]++;
                        i++;
                        break;
                    }
                }
            }
        }// else
        return num;
    }

    public static double calculate_average(double[] list) {
        double sum = 0;

        for (int i = 0; i < list.length; i++) {
            sum += list[i];
        }

        return sum / list.length;
    }

    public static double calculate_var(double[] list) {
        double sum = 0;
        double avg = calculate_average(list);

        for (int i = 0; i < list.length; i++) {
            sum += Math.pow(Math.abs(list[i] - avg), 2);
        }

        return sum / list.length;
    }

    public static void print_file(String filename, double[] x, double[] y) 
    throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
        int size = x.length;
        for (int i = 0; i < size; i++) {
            writer.write(x[i] + " " + y[i]);
            writer.newLine();
        }
        writer.close();
  }

}