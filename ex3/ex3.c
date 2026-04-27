#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
int main(int argc,char* argv[]){
        int my_rank,comm_sz;
        int n,num;
        int *arr=NULL,*sub=NULL;
        int sub_size;
        int local_count=0,total_count=0;
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
        if(my_rank==0){
                printf("Enter the number of elements :");
                fflush(stdout);
                scanf("%d",&n);
                arr=(int*)malloc(n* sizeof(int));
                printf("Array elements:");
                int i;
                for(i=0;i<n;i++){
                        arr[i]=rand()%10;
                        printf("%d\t",arr[i]);
                }

                printf("\n");
                printf("Enter the number to search:");
                fflush(stdout);
                scanf("%d",&num);
        }
                MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
                MPI_Bcast(&num,1,MPI_INT,0,MPI_COMM_WORLD);
                sub_size=n/comm_sz;
                sub=(int*)malloc(sub_size*sizeof(int));
                MPI_Scatter(arr,sub_size,MPI_INT,sub,sub_size,MPI_INT,0,MPI_COMM_WORLD);
                local_count=0;
                int i;
                for(i=0;i<sub_size;i++)
                        if(sub[i]==num)
                                local_count++;
                MPI_Reduce(&local_count,&total_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
                if(my_rank==0){
                        printf("Frequency of %d is %d \n",num,total_count);
                }
                MPI_Finalize();
                return 0;
}
