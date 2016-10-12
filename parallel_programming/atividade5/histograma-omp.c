#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>


typedef struct _histogram {
  size_t num_bins;    /* number of bins */
  size_t *bins;       /* size of each bin */
  double min_value;
  double max_value;
  double bin_width;    /* width of each bin */
                      /* not really needed, but can speed things up */
} histogram_t;

/*
 * value: value to search
 * hist: histogram data structure containing the bins to be searched
 *
 * returns the bin index of 'value'
 */
size_t search_for_bin(double value, histogram_t *hist)
{
  size_t i = 0;
  while (i < hist->num_bins && value >= (i+1)*hist->bin_width) i++;

  // condition checks the case where there is a value equal to max_value
  return i >= hist->num_bins ? hist->num_bins-1 : i;
}

/*
 * Input:
 *
 * data: array of values (double) from which the histogram will be created
 * data_count: number of elements in 'data'
 * min_meas: minimum value
 * max_meas: maximum value
 * bin_count: number of bins
 *
 * Output:
 *
 * the created histogram
 *
 *
 */
histogram_t *create_histogram(double *data, size_t data_count, double min_meas, double max_meas, size_t bin_count)
{
  size_t i;

  histogram_t *hist = (histogram_t *) malloc(sizeof(histogram_t));
  if (hist == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  hist->num_bins = bin_count;
  hist->min_value = min_meas;
  hist->max_value = max_meas;
  hist->bin_width = (max_meas - min_meas)/bin_count;
  if ((hist->bins = ((size_t *)calloc(bin_count, sizeof(size_t)))) == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }
# pragma omp parallel for default(none) shared(data, hist, data_count, i)
  for (i=0; i<data_count; i++)
  {
    size_t bin_index = search_for_bin(data[i], hist);
    hist->bins[bin_index]++;
  }

  return hist;
}

/*
 * Print the histogram.
 *
 * Show, for each bin, the number of elements.
 *
 */
void print_histogram(histogram_t *hist)
{
  size_t i;

  for (i=0; i<hist->num_bins; i++)
    fprintf(stdout, "bin[%zu] = %zu\n", i, hist->bins[i]);
}

double *data;

/*
 * File format:
 *    count
 *    min
 *    max
 *    <value 0>
 *    <value 1>
 *    ...
 *    <value count-1>
 */
void read_data(char *filename, size_t *count, double *min, double *max)
{
  FILE *fp;
  if ((fp = fopen(filename, "r")) == NULL) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  char buf[100];
  fgets(buf, 100, fp);
  *count = strtol(buf, NULL, 10);
  fgets(buf, 100, fp);
  *min = strtod(buf, NULL);
  fgets(buf, 100, fp);
  *max = strtod(buf, NULL);

  if ((data = (double *)malloc(*count*sizeof(double))) == NULL) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  size_t i;
  for (i=0; i<*count; i++) {
    fgets(buf, 100, fp);
    data[i] = strtod(buf, NULL);
//    fprintf(stdout, "%g\n", data[i]);
  }

  fclose(fp);
}



int main(int argc, char *argv[])
{
  histogram_t *myhist;
  struct timeval start, stop;

  size_t count = 0;
  double min, max;

  /* <app> bin_count file-name */
  if (argc < 3) {
    fprintf(stderr, "Invalid number of parameters\n");
    exit(EXIT_FAILURE);
  }

  char *filename;
  size_t bin_count = strtol(argv[1], NULL, 10);
  filename = argv[2];

  read_data(filename, &count, &min, &max);

  gettimeofday(&start, NULL);
  myhist = create_histogram(data, count, min, max, bin_count);
  gettimeofday(&stop, NULL);

  double t = (((double)(stop.tv_sec)*1000.0  + (double)(stop.tv_usec / 1000.0)) - \
                   ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec / 1000.0)));

  fprintf(stdout, "Time elapsed = %g ms\n", t);

  print_histogram(myhist);

  // missing: deallocate memory

  return 0;
}
