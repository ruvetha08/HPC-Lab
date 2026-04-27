#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
typedef struct student{
        char name[30];
        int roll_no;
        float total_marks;
        char grade;
}student;

int main(int argc,char** argv)
{
        int my_rank,comm_sz;
        int total_students;
        MPI_INIT(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
        MPI_Cmm_Size(MPI_COMM_WORLD,&comm_sz);
        if(my_rank==0)
        {
                FILE *fp=fopen("input.txt",'r');
                if(fp==NULL)
                {
                        printf("Error:Could not open input.txt");
                        total_students=1;
                }
                else
                {
                        fscanf(fp,"%d",&total_students);
                        fclose(fp);
                }
        }
        MPI_Bcast(&total_students,1,MPI_Int,0,MPI_COMM_WORLD);
        if(total_students==-1)
        {
                MPI_finalize();
                return 0;
        }
        int n_per_proc=total_students/comm_sz;
        student *all_students=NULL;
        student *loal_students=malloc(n_per_proc* sizeof(students));
        MPI_Datatype student_mpi_t;
        int b[4]={20,1,1,1};
        MPI_Datatype types[4]={MPI_CHAR,MPI_INT,MPI_FLOAT,MPI_CHAR};
        MPI_Aint displacement[4];
        student sample;
        MPI_Aintbase_addr,name_addr,roll_addr,mark_addr,grade_addr;
        MPI_Get_address(&sample,&base_addr);
        MPI_Get_address(&sample.name,&name_addr);
        MPI_Get_address(&sample.roll_no,&roll_addr);
        MPI_Get_address(&sample.total_marks,&marks_addr);
        MPI_Get_address(&sample.grade,&grade_addr);
        displacement[0]=name_addr-base_addr;
        displacement[1]=roll_addr-base_addr;
        displacement[2]=marks_addr-base_addr;
        displacement[3]=grade_addr-base_addr;
        MPI_Type_create_Struct(4,b,displacement.types,&student_mpi_t);
        MPI_Type_Commit(&student_mpi_t);
        if(rank==0)
        {
                all_students=maaloc(total_students *sizeof(student));
                FILE *fp=fopen("input.txt");
                int dummy;
                fscanf(fp,"%d",&dummy);
                for(int i=0;i<total_students;i++)
                {
                        scanf(fp,"%s %d %f",all_students[i].name,&all_students[i],roll_no,&all_students[i],total_marks);
                }
                fclose(fp);
        }
        MPI_Scatter(all_students,n_per_proc,student_mpi_t,local_student,
                        n_per_proc,student_mpi_t,0,MPI_COMM_WORLD);
        for(int i=0;i<n_per_proc;i++)
        {
                if(local_students[i].total_marks>=85)
                {
                        local_student[i].grade='s';
                }
                else if(local_student[i].total_marks>=70)
                {
                        local_student[i].grade='b';
                }
                else
                {
                        local_student[i].grade='f';
                }
                MPI_Gather(local_student,n_per_proc,student_mpi_t,all_students,n_per_proc,student_mpi_t,0,MPI_COMM_WORLD);
                if(my_rank==0)
                {
                        FILE *out=fopen("grage.txt","w");
                        for(int i=0;i<total_students;i++)
                        {
                                fprintf(out,"Roll:%d|name:%s|grade:%c\n",all_students[i].roll_no,all_students[i].name,all_students[i].grade);
                        }
                        fclose(out);
                        printf("Processed %d students result in grades.txt\n",total_students);
                        free(students);
                }
                free(total_students);
                MPI_Type_free(&student_mpi_t);
                MPI_Finalize();
                return 0;
        }
}
