#include "Matrix.h"

Matrix* read_line()
{
    int line=1, col=1, temp=1, i=0;
    char input[1024], clean_data[1024], *pointer = NULL;
    Matrix *mat= NULL;

    printf("Enter matrix: ");
    gets(input);

    // Delete spaces and () at the beginning and the end
    pointer = input;
    while(*pointer ==' ' || *pointer =='(')
        pointer++;
    strcpy(input,pointer);
    pointer =input + strlen(input)-1;
    while(*pointer ==' ' || *pointer ==')')
       pointer--;
    *(pointer+1)= '\0';

    // Replace double spaces by space and delete spaces around ;
    pointer = input;
    while(*pointer != '\0')
    {
        if(*pointer==' ' && (*(pointer+1)==' ' || *(pointer+1) ==';' || *(pointer-1)==';' ))
        {
            pointer++;
            continue;
        }

        clean_data[i]=*pointer;
        pointer++;
        i++;
    }
    clean_data[i]='\0';


// Count columns = Number of spaces before a semi-colon +1
    pointer = clean_data;
    while(*pointer!=';' && *pointer!='\0')
    {
        if(*pointer==' ')
            col++;
        pointer++;
    }

// Every line should have col columns
// Count lines and check that each of them have col columns
pointer=clean_data;
while(*pointer !='\0')
{
    if(*pointer==' ')
        temp++;
    if(*pointer==';'&&(temp!=col))
        exit(EXIT_FAILURE);
    if(*pointer==';'&&(temp==col))
        {
            temp = 1;
            line++;
            *pointer =' '; // Replace semi-colon by space
        }
    pointer++;
}

if(temp != col) // For the last line
    exit(EXIT_FAILURE);
mat = create_matrix(line,col);
fill_matrix(mat,clean_data);

return mat;
}

void fill_matrix(Matrix *mat, char *clean_data)
{
    int i = 0, j=0;
    char *pointer=strtok(clean_data," ");

    while(pointer!=NULL)
    {
       (mat->values)[i][j]= atof(pointer);
       j++;
       if(j==mat->col)
       {
        j =0 ;
        i++;
       }
       pointer = strtok(NULL," ");
    }
}

Matrix* inverse_matrix(Matrix *mat)
{
    int i = 0, j =0;
    double det = 0.0;
    Matrix *inverse = NULL, *comatrix = NULL;

    det = det_matrix(mat);
    if(det ==0)
    {
        printf("The matrix has no inverse \n");
        exit(EXIT_FAILURE);
    }

    comatrix = com_matrix(mat);
    inverse = transpose_matrix(comatrix);

    for(i=0;i<inverse->line;i++)
    {
        for(j=0; j<inverse->col;j++)
          (inverse->values)[i][j] = (inverse->values)[i][j]/det;
    }

    free_matrix(comatrix);
    return inverse;
}

Matrix* com_matrix(Matrix *mat)
{
    int i =0, j = 0, coef = 0;
    Matrix *comatrix = NULL, *reduce = NULL;

    comatrix = create_matrix(mat->line, mat->col);

    for(i=0;i<mat->line;i++)
    {
        for(j=0; j<mat->col;j++)
        {
            coef = ((i+j)%2)? -1:1;
            reduce = reduce_matrix(mat,i,j);
            (comatrix->values)[i][j] = coef*det_matrix(reduce);
            free_matrix(reduce);
        }
    }
    return comatrix;
}

double det_matrix(Matrix *mat)
{
    int i=0, coeff=0;
    double det = 0.0;
    Matrix *residu = NULL;

    operation_checker(mat->line, mat->col);

    if(mat->line==1)
        return (mat->values)[0][0];

    else if(mat->line == 2)
         det = (mat->values)[0][0]*(mat->values)[1][1]-(mat->values)[0][1]*(mat->values)[1][0];

     else
     {
         for(i =0; i< mat->line; i++)
         {
             coeff = (i%2)?-1:1;
             residu = reduce_matrix(mat,i,0);
             det += det_matrix(residu)*coeff*(mat->values)[i][0];
             free_matrix(residu);
         }
     }

    return det;
}

Matrix* transpose_matrix(Matrix *mat)
{
    int i = 0, j = 0;
    Matrix *trans = NULL;

    trans = create_matrix(mat->col, mat->line);

    for (i =0; i< trans->line; i++)
        for (j =0; j< trans->col; j++)
            (trans->values)[i][j] = (mat->values)[j][i];

    return trans;
}


Matrix* reduce_matrix(Matrix *mat,int line, int col)
{
    int i=0, j=0, p =0, q=0;
    Matrix *reduce = NULL;

    reduce = create_matrix((mat->line)-1, (mat->col)-1);
    for (i =0; i< mat->line; i++)
    {
        if(i == line)
            continue;
        q = 0;
        for (j =0; j< mat->col; j++)
        {
            if(j == col)
                continue;
            (reduce->values)[p][q] = (mat->values)[i][j];
            q++;
        }
        p++;
    }

    return reduce;
}


Matrix* create_matrix(int line, int col)
{
     int i = 0;
    Matrix *mat = NULL;

    mat = (Matrix*) calloc(1,sizeof(Matrix));
    mat->line = line, mat->col = col;
    mat->values = (double**) calloc(line, sizeof(double*));
    for(i = 0; i<line; i++)
        (mat->values)[i] = (double*) calloc(col,sizeof(double));

    return mat;
}

void free_matrix(Matrix *mat)
{
    int i =0;
    for(i = 0; i< mat->line; i++)
        free((mat->values)[i]);
    free(mat->values);
    free(mat);
}

void display_matrix(Matrix *mat)
{
   int i=0, j=0;
    for(i = 0; i<mat->line; i++)
    {
        for(j = 0; j<mat->col; j++)
            printf("%3f\t",(mat->values)[i][j]);
        printf("\n\n");
    }
}

Matrix* product_matrix(Matrix *mat1, Matrix *mat2)
{
    int i =0, j=0, k=0;
    Matrix *prod = NULL;

    operation_checker(mat1->col, mat2->line);

    prod = create_matrix(mat1->line, mat2->col);

    for(i = 0; i<prod->line; i++)
    {
        for(j = 0; j<prod->col; j++)
        {
            for(k = 0; k<mat1->col; k++)
                (prod->values)[i][j] += (mat1->values)[i][k]*(mat2->values)[k][j];
        }
    }
   return prod;
}

Matrix* sum_matrix(Matrix *mat1, Matrix *mat2)
{
    int i = 0, j = 0;
    Matrix *sum = NULL;

    operation_checker(mat1->line, mat2->line);
    operation_checker(mat1->col, mat2->col);

    sum = create_matrix(mat1->line, mat1->col);

    for(i = 0; i<mat1->line; i++)
    {
        for(j = 0; j<mat1->col; j++)
            (sum->values)[i][j] = (mat1->values)[i][j] + (mat2->values)[i][j] ;
    }

   return sum;
}

void operation_checker(int line, int col)
{
    if(line != col)
    {
        printf("Impossible operation with such entries \n");
        exit(EXIT_FAILURE);
    }
}

void copy_matrix(Matrix *mat1, Matrix *mat2)
{
    int i, j;

    for(i=0;i<mat1->line;i++)
        for(j=0;j<mat1->col;j++)
            (mat1->values)[i][j]=(mat2->values)[i][j];
}

void pow_matrix(Matrix *mat, int n)
{
    Matrix *temp = create_matrix(mat->line,mat->col);
    Matrix *prod;

    if(n<1)
        exit(-1);
    else if(n==1)
        display_matrix(mat);
    else
    {
        prod = product_matrix(mat,mat);
        n --;
        while(n>1)
        {
            copy_matrix(temp,prod);
            free_matrix(prod); // free memory because product_matrix create allocate a new memory
            prod = product_matrix(temp,mat);
            n--;
        }
    }

    display_matrix(prod);
    free_matrix(prod);
    free_matrix(temp);
}
