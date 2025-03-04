#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define MODULO 10007

#define DIE(condition, message) \
	do { \
		if (condition) { \
			fprintf(stderr, "%s - %s:%d: %s\n", \
				message, __FILE__, __LINE__, strerror(errno)); \
			exit(EXIT_FAILURE); \
		} \
	} while (0)
typedef struct {
	int **matrix;
	int m;
	int n;
} array_of_matrix;

void resizable_array(array_of_matrix **array, int *nr_matrix, int index)
{
	if (*nr_matrix <= index) {
		int old_size = *nr_matrix;
		int new_size = (*nr_matrix == 0) ? 1 : *nr_matrix * 2;
		*array = (array_of_matrix *)realloc
		(*array, sizeof(array_of_matrix) * new_size);
		DIE(!(*array), "Failed to reallocate array");

		// IniTializeaza noile elemente adaugate in array
		for (int i = old_size; i < new_size; i++) {
			(*array)[i].matrix = NULL;
			(*array)[i].m = 0;
			(*array)[i].n = 0;
		}

		*nr_matrix = new_size;
	}
}

void new_matrix(array_of_matrix **array, int m, int n, int index)
{
	int i;
	(*array)[index].matrix = (int **)malloc(m * sizeof(int *));
	DIE(!((*array)[index].matrix), "Error allocating memory for rows");
	for (i = 0; i < m; i++) {
		(*array)[index].matrix[i] = (int *)malloc(n * sizeof(int));
		 // If allocation fails, free already allocated memory
		if (!((*array)[index].matrix[i])) {
			for (int j = 0; j < i; j++) {
				free((*array)[index].matrix[j]);
				(*array)[index].matrix[j] = NULL;
			}
			free((*array)[index].matrix);
			(*array)[index].matrix = NULL;
			DIE(1, "Error allocating memory for a row");
		}
	}
	(*array)[index].m = m;
	(*array)[index].n = n;
}

void read_matrix(array_of_matrix *array, int index)
{
	for (int i = 0; i < array[index].m; i++) {
		for (int j = 0; j < array[index].n; j++)
			scanf("%d", &array[index].matrix[i][j]);
	}
}

// Function to transpose a matrix
void transpus_matrix(array_of_matrix *array, int index)
{
	int m = array[index].m;
	int n = array[index].n;
	// Allocate memory for the transposed matrix
	int **transposed = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++) {
		transposed[i] = (int *)malloc(m * sizeof(int));
		for (int j = 0; j < m; j++)
			transposed[i][j] = array[index].matrix[j][i];
	}
	// Free the original matrix and replace with the transposed one
	for (int i = 0; i < m; i++) {
		free(array[index].matrix[i]);
		array[index].matrix[i] = NULL;
	}
	free(array[index].matrix);
	array[index].matrix = NULL;

	array[index].matrix = transposed;
	array[index].m = n;
	array[index].n = m;
}

// Function to free memory allocated for all matrices
void free_matrix(array_of_matrix *array, int index)
{
	for (int i = 0; i < index; i++) {
		if (array[i].matrix) {
			for (int j = 0; j < array[i].m; j++) {
				if (array[i].matrix[j]) {
					free(array[i].matrix[j]);
					array[i].matrix[j] = NULL;
				}
			}
			free(array[i].matrix);
			array[i].matrix = NULL;
		}
	}
}

// Function to print a matrix to standard output
void printf_matrix(array_of_matrix *array, int index)
{
	for (int i = 0; i < array[index].m; i++) {
		for (int j = 0; j < array[index].n; j++)
			printf("%d ", array[index].matrix[i][j]);
		printf("\n");
	}
}

void multiply_matrix(array_of_matrix **array, int *nr_matrix, int index1,
					 int index2, int *index)
{
	// Check if the indices are within the valid range
	if (index1 < 0 || index1 >= *index || index2 < 0 || index2 >= *index) {
		printf("No matrix with the given index\n");
		return;
	}
	// Check if matrix multiplication is possible
	//(columns of first matrix == rows of second)
	if ((*array)[index1].n != (*array)[index2].m) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int m = (*array)[index1].m, n = (*array)[index2].n, p = (*array)[index1].n;
	resizable_array(array, nr_matrix, *index);
	int new_index = *index;
	// Create a new matrix to store the result
	new_matrix(array, m, n, new_index);

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			(*array)[new_index].matrix[i][j] = 0;
			for (int k = 0; k < p; k++) {
				int product = ((*array)[index1].matrix[i][k] *
				(*array)[index2].matrix[k][j]) % MODULO;
				if (product < 0)
					product += MODULO;
				(*array)[new_index].matrix[i][j] = ((*array)
				[new_index].matrix[i][j] + product) % MODULO;
			}
		}
	}
	// Increment the index for the next matrix
	(*index)++;
	// Set the dimensions of the new matrix
	(*array)[new_index].m = m;
	(*array)[new_index].n = n;
}

// function for AxB
void A_x_B(int **mat1, int **mat2, int ***result, int m, int n)
{
	int **aux;
	aux = (int **)calloc(m, sizeof(int *));
	for (int i = 0; i < m; i++) {
		aux[i] = (int *)calloc(n, sizeof(int));
		if (!aux[i]) {
			printf("Eroare la alocarea memoriei pentru matricea aux");
			exit(EXIT_FAILURE);
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < m; k++) {
				aux[i][j] += mat1[i][k] * mat2[k][j];
				aux[i][j] %= MODULO;
				if (aux[i][j] < 0)
					aux[i][j] += MODULO;
			}
		}
	}
	//I free the first matrix because it will be
	*result = aux;
}

void free_specific_matrix(array_of_matrix *matrix)
{
	for (int j = 0; j < matrix->m; j++) {
		if (matrix->matrix[j]) {
			free(matrix->matrix[j]);
			matrix->matrix[j] = NULL;
		}
	}
	if (matrix->matrix) {
		free(matrix->matrix);
		matrix->matrix = NULL;
	}
}

void reallocate_matrix_array_if_needed(array_of_matrix **arr,
									   int *nr_matrix, int index)
{
	if (index * 2 < *nr_matrix) {
		int new_size = index > 0 ? index : 1;
		*arr = (array_of_matrix *)realloc(*arr, new_size
		 * sizeof(array_of_matrix));
		if (!(*arr) && new_size > 0) {
			fprintf(stderr, "Reallocation failed.\n");
			exit(EXIT_FAILURE);
		}
		*nr_matrix = new_size;
	}
}

void elim_matrix(array_of_matrix **arr, int *index, int *nr_matrix)
{
	// Request index of the matrix to be eliminated
	int index_request;
	scanf("%d", &index_request);
	if (index_request < 0 || index_request >= *index) {
		printf("No matrix with the given index\n");
		return;
	}

	free_specific_matrix(&((*arr)[index_request]));
	// Shift all matrices after the eliminated one to fill the gap
	for (int i = index_request; i < *nr_matrix - 1; i++)
		(*arr)[i] = (*arr)[i + 1];
	// Decrement the count of matrices since one has been removed
	(*index)--;
	reallocate_matrix_array_if_needed(arr, nr_matrix, *index);
}

int matrix_sum(array_of_matrix *matrix)
{
	int sum = 0;
	for (int i = 0; i < matrix->m; i++) {
		for (int j = 0; j < matrix->n; j++) {
			sum += matrix->matrix[i][j];
			sum = sum % MODULO;
			if (sum < 0)
				sum += MODULO;
		}
	}
	return sum;
}

void sort_matrices(array_of_matrix *array, int index)
{
	for (int i = 0; i < index; i++) {
		for (int j = i + 1; j < index; j++) {
			if (matrix_sum(&array[i]) > matrix_sum(&array[j])) {
				// Swap matrices
				array_of_matrix aux = array[i];
				array[i] = array[j];
				array[j] = aux;
			}
		}
	}
}

// function for resize a matrix
void resize_matrix(array_of_matrix *array, int index)
{
	int index_request;
	scanf("%d", &index_request);
	// Read row indices
	int number_rows;
	scanf("%d", &number_rows);
	int *array_rows = (int *)malloc(number_rows * sizeof(int));
	for (int i = 0; i < number_rows; ++i)
		scanf("%d", &array_rows[i]);

	// Read column indices
	int number_cols;
	scanf("%d", &number_cols);
	int *array_cols = (int *)malloc(number_cols * sizeof(int));
	for (int i = 0; i < number_cols; ++i)
		scanf("%d", &array_cols[i]);

	// Check index validity
	if (index_request < 0 || index_request >= index) {
		printf("No matrix with the given index\n");
		free(array_rows);
		free(array_cols);
		return;
	}

	// Allocate new matrix
	int **new_matrix = (int **)malloc(number_rows * sizeof(int *));
	for (int i = 0; i < number_rows; i++) {
		new_matrix[i] = (int *)malloc(number_cols * sizeof(int));
		for (int j = 0; j < number_cols; j++) {
			int rows;
			rows = array_rows[i];
			int cols = array_cols[j];
			new_matrix[i][j] = array[index_request].matrix[rows][cols];
		}
	}

	// Free old matrix and assign new matrix
	for (int i = 0; i < array[index_request].m; i++)
		free(array[index_request].matrix[i]);
	free(array[index_request].matrix);

	array[index_request].matrix = new_matrix;
	array[index_request].m = number_rows;
	array[index_request].n = number_cols;

	// Free temporary arrays
	free(array_rows);
	free(array_cols);
}

int **allocate_matrix(int n)
{
	int **matrix = (int **)malloc(n * sizeof(int *));
	for (int i = 0; i < n; i++)
		matrix[i] = (int *)malloc(n * sizeof(int));
	return matrix;
}

void free_matrix_data(int **matrix, int n)
{
	for (int i = 0; i < n; i++) {
		if (matrix[i])
			free(matrix[i]);
	}
	if (matrix)
		free(matrix);
}

void raise_matrix_power(array_of_matrix *array, int index)
{
	int matrix_index, power;
	scanf("%d %d", &matrix_index, &power);

	if (matrix_index < 0 || matrix_index >= index) {
		printf("No matrix with the given index\n");
		return;
	}

	if (power < 0) {
		printf("Power should be positive\n");
		return;
	}

	if (array[matrix_index].m != array[matrix_index].n) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}

	int m = array[matrix_index].m;

	// Allocate separate memory for base and copy the original matrix
	int **base = allocate_matrix(m);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			base[i][j] = array[matrix_index].matrix[i][j];

	// Allocate result matrix
	int **result = allocate_matrix(m);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			// Initialize with the identity matrix
			result[i][j] = (i == j) ? 1 : 0;

	// Apply the fast exponentiation algorithm
	while (power > 0) {
		if (power % 2 == 1) {
			int **temp_result = NULL;
			A_x_B(result, base, &temp_result, m, m);
			free_matrix_data(result, m); // Free the old result
			result = temp_result;
		}

		int **temp_base = NULL;
		A_x_B(base, base, &temp_base, m, m);
		free_matrix_data(base, m); // Free the old base
		base = temp_base;

		power /= 2;
	}

	// Copy the result back to the original matrix
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			array[matrix_index].matrix[i][j] = result[i][j];

	// Free the allocated matrices
	free_matrix_data(result, m);
	free_matrix_data(base, m);
}

int main(void)
{
	char character;
	// Counters for number of matrices and current index
	int nr_matrix = 0, index = 0;
	array_of_matrix *array = NULL;
	int m, n, index_request, m1_i, m2_i;

	while (1) {
		scanf(" %c", &character);
			switch (character) {
			case 'L':
				scanf("%d%d", &m, &n);
				resizable_array(&array, &nr_matrix, index);
				new_matrix(&array, m, n, index);
				read_matrix(array, index);
				index++;
				break;
			case 'D':
				scanf("%d", &index_request);
				if (index_request < 0 || index_request >= index) {
					printf("No matrix with the given index\n");
				} else {
					printf("%d ", array[index_request].m);
					printf("%d\n", array[index_request].n);
				}
				break;
			case 'P':
				scanf("%d", &index_request);
				if (index_request < 0 || index_request >= index)
					printf("No matrix with the given index\n");
				else
					printf_matrix(array, index_request);
				break;
			case 'M':
				scanf("%d%d", &m1_i, &m2_i);
				multiply_matrix(&array, &nr_matrix, m1_i, m2_i, &index);
				break;
			case 'T':
				scanf("%d", &index_request);
				if (index_request < 0 || index_request >= index)
					printf("No matrix with the given index\n");
				else
					transpus_matrix(array, index_request);
				break;
			case 'F':
				elim_matrix(&array, &index, &nr_matrix);
				break;
			case 'O':
				sort_matrices(array, index);
				break;
			case 'C':
				resize_matrix(array, index);
				break;
			case 'R':
				raise_matrix_power(array, index);
				break;
			case 'Q':
				goto end;
			default:
				printf("Unrecognized command\n");
				break;
		}
	}
end:
	free_matrix(array, index);
	free(array);
	return 0;
}
