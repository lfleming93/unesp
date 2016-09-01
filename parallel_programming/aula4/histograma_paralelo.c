#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>


typedef struct _histogram {
  size_t num_bins;    /* number of bins */
  size_t *bins;       /* size of each bin */
  double min_value;
  double max_value;
  double bin_width;    /* width of each bin */
                      /* not really needed, but can speed things up */
} histogram_t;

histogram_t *hist = NULL;

pthread_mutex_t mutexsum;

size_t count = 0;
int thread_count;
int flag = 0;
double *data;

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

void* create_histogram_parallel(void* rank) {
  long my_rank = (long) rank;
  unsigned long i, my_sum;
  unsigned long local_N = count / thread_count;
  unsigned long first_i = my_rank * local_N;
  unsigned long last_i = (my_rank + 1) * local_N - 1;

  for (i = first_i; i <= last_i; i++)
  {
    size_t bin_index = search_for_bin(data[i], hist);

    pthread_mutex_lock (&mutexsum);
    hist->bins[bin_index]++;
    pthread_mutex_unlock (&mutexsum);


  }

  return NULL;
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
histogram_t *create_histogram(double min_meas, double max_meas, size_t bin_count, int thread_count)
{
  size_t i;
  long thread;

  hist = (histogram_t *) malloc(sizeof(histogram_t));
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

  pthread_t* thread_handles = malloc (thread_count*sizeof(pthread_t));

  for (thread = 0; thread < thread_count; thread++)
    pthread_create(&thread_handles[thread], NULL, create_histogram_parallel, (void*) thread);


  for (thread = 0; thread < thread_count; thread++)
    pthread_join(thread_handles[thread], NULL);

  free(thread_handles);

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

  double min, max;

  /* <app> bin_count file-name */
  if (argc < 4) {
    fprintf(stderr, "Invalid number of parameters\n");
    exit(EXIT_FAILURE);
  }

  char *filename;
  size_t bin_count = strtol(argv[1], NULL, 10);
  filename = argv[2];
  thread_count = atoi(argv[3]);

  read_data(filename, &count, &min, &max);

  pthread_mutex_init(&mutexsum, NULL);

  gettimeofday(&start, NULL);
  myhist = create_histogram(min, max, bin_count, thread_count);
  gettimeofday(&stop, NULL);

  double t = (((double)(stop.tv_sec)*1000.0  + (double)(stop.tv_usec / 1000.0)) - \
                   ((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec / 1000.0)));

  fprintf(stdout, "Time elapsed = %g ms\n", t);

  print_histogram(myhist);

  // missing: deallocate memory

  return 0;
}
