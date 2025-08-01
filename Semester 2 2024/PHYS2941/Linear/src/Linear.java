//   Script  to  calculate the Linear regression plot
//   Written by  Samuel Allpass
//   Version 1
//   4th August 2024

import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class Linear {
    private List<Float> x;
    private List<Float> y;
    private List<Float> uncerty;
    private boolean isWeighted;
    private Float m;
    private Float c;
    private static String xname;
    private static String yname;

    public Linear(List<Float> x, List<Float> y, List<Float> uncerty, boolean isWeighted) {
        this.x = x;
        this.y = y;
        this.uncerty = uncerty;
        this.isWeighted = isWeighted;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        try {
            System.out.println("What is the name of the x data:");
            xname = scanner.nextLine();
            System.out.println("What is the name of the y data:");
            yname = scanner.nextLine();

            System.out.println("Enter the first (x) data set:");
            List<Float> x = Arrays.stream(scanner.nextLine().split(","))
                                  .map(Float::parseFloat)
                                  .collect(Collectors.toList());

            System.out.println("Enter the second (y) data set:");
            List<Float> y = Arrays.stream(scanner.nextLine().split(","))
                                  .map(Float::parseFloat)
                                  .collect(Collectors.toList());

            System.out.println("Enter the uncertainty of the second (y) data set:");
            List<Float> uncerty = Arrays.stream(scanner.nextLine().split(","))
                                        .map(Float::parseFloat)
                                        .collect(Collectors.toList());

            System.out.println("Is the regression Weighted (true or false):");
            boolean isWeighted = Boolean.parseBoolean(scanner.nextLine());

            scanner.close();

            Linear linear = new Linear(x, y, uncerty, isWeighted);
            linear.performRegression();
        } catch (Exception e) {
            System.err.println("Invalid input. Please enter valid numbers.");
        }
    }

    private void performRegression() {
        float deltam = 0, deltac = 0;
    
        if (isWeighted) {
            m = weightedGradientCalc();
            deltam = weightedGradientUncertCalc();
            c = weightedYintCalc();
            deltac = weightedYintUncertCalc();
        } else {
            m = gradientCalc();
            deltam = gradientUncertCalc();
            c = yintCalc();
            deltac = yintUncertCalc();
        }
    
        System.out.printf("y = (%.5f ± %.5f)x + (%.5f ± %.5f)%n", m, deltam, c, deltac);

        saveDataToDat();
    }
    
    private void saveDataToDat() {
        try (PrintWriter writer = new PrintWriter(new FileWriter("src/data.dat"))) {
            for (int i = 0; i < x.size(); i++) {
                writer.printf("%f, %f, %f%n", x.get(i), y.get(i), uncerty.get(i));
            }
            writer.printf("%s\n", xname);
            writer.printf("%s\n", yname);
            writer.printf("%.2f\n", m);
            writer.printf("%.2f", c);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    

    private float weightedMeanCalc(List<Float> f, List<Float> uncert) {
        float numerator = 0f;
        float denominator = 0f;
        for (int i = 0; i < f.size(); i++) {
            float weight = 1 / (uncert.get(i) * uncert.get(i));
            numerator += f.get(i) * weight;
            denominator += weight;
        }
        return numerator / denominator;
    }

    private float weightedGradientCalc() {
        float sum = 0f;
        float meanX = weightedMeanCalc(x, uncerty);
        for (int i = 0; i < x.size(); i++) {
            float weight = 1 / (uncerty.get(i) * uncerty.get(i));
            sum += weight * (x.get(i) - meanX) * y.get(i);
        }
        return sum / weightedDcalc();
    }

    private float weightedDcalc() {
        float sum = 0f;
        float meanX = weightedMeanCalc(x, uncerty);
        for (int i = 0; i < x.size(); i++) {
            float weight = 1 / (uncerty.get(i) * uncerty.get(i));
            sum += weight * (x.get(i) - meanX) * (x.get(i) - meanX);
        }
        return sum;
    }

    private float weightedYintCalc() {
        return weightedMeanCalc(y, uncerty) - weightedGradientCalc() * weightedMeanCalc(x, uncerty);
    }

    private float weightedGradientUncertCalc() {
        float sum = 0f;
        float gradient = weightedGradientCalc();
        float intercept = weightedYintCalc();

        for (int i = 0; i < x.size(); i++) {
            float weight = 1 / (uncerty.get(i) * uncerty.get(i));
            float residual = y.get(i) - gradient * x.get(i) - intercept;
            sum += weight * residual * residual;
        }

        float deltamsq = (1 / weightedDcalc()) * (sum / (x.size() - 2));
        return (float) Math.sqrt(deltamsq);
    }

    private float weightedYintUncertCalc() {
        float sum = 0f;
        float sumw = 0f;
        float gradient = weightedGradientCalc();
        float intercept = weightedYintCalc();
        float meanX = weightedMeanCalc(x, uncerty);

        for (int i = 0; i < x.size(); i++) {
            sumw += 1 / (uncerty.get(i) * uncerty.get(i));
        }

        for (int i = 0; i < x.size(); i++) {
            float weight = 1 / (uncerty.get(i) * uncerty.get(i));
            float residual = y.get(i) - gradient * x.get(i) - intercept;
            sum += weight * residual * residual;
        }

        float deltacsq = ((1 / sumw) + ((meanX * meanX) / weightedDcalc())) * (sum / (x.size() - 2));
        return (float) Math.sqrt(deltacsq);
    }

    private float meanCalc(List<Float> f) {
        float sum = 0f;
        for (Float value : f) {
            sum += value;
        }
        return sum / f.size();
    }

    private float gradientCalc() {
        float sum = 0f;
        float meanX = meanCalc(x);
        for (int i = 0; i < x.size(); i++) {
            sum += (x.get(i) - meanX) * y.get(i);
        }
        return sum / dcalc();
    }

    private float dcalc() {
        float sum = 0f;
        float meanX = meanCalc(x);
        for (int i = 0; i < x.size(); i++) {
            sum += (x.get(i) - meanX) * (x.get(i) - meanX);
        }
        return sum;
    }

    private float yintCalc() {
        return meanCalc(y) - gradientCalc() * meanCalc(x);
    }

    private float gradientUncertCalc() {
        float sum = 0f;
        float gradient = gradientCalc();
        float intercept = yintCalc();

        for (int i = 0; i < x.size(); i++) {
            float residual = y.get(i) - gradient * x.get(i) - intercept;
            sum += residual * residual;
        }

        float deltamsq = (1 / dcalc()) * (sum / (x.size() - 2));
        return (float) Math.sqrt(deltamsq);
    }

    private float yintUncertCalc() {
        float sum = 0f;
        float gradient = gradientCalc();
        float intercept = yintCalc();
        float meanX = meanCalc(x);

        for (int i = 0; i < x.size(); i++) {
            float residual = y.get(i) - gradient * x.get(i) - intercept;
            sum += residual * residual;
        }

        float deltacsq = ((1 / x.size()) + ((meanX * meanX) / dcalc())) * (sum / (x.size() - 2));
        return (float) Math.sqrt(deltacsq);
    }
}




