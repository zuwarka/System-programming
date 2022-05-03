#include <stdio.h>
void num_of_parameters(){
	int param;
	asm(
		"movl $0,%%eax\n"
		"cpuid\n"
		"movl %%eax,%0\n"

		:"=a"(param)
	);
	printf("Number of parametrs %d",param);
}
void prod(){
	int reg_b,reg_c,reg_d,i;
	asm(
		"movl $0,%%eax\n"
		"cpuid\n"
		"movl %%ebx,%0\n"
		"movl %%ecx,%1\n"
		"movl %%edx,%2\n"
		:"=b"(reg_b),"=c"(reg_c),"=d"(reg_d)
	);

	for(i=0;i<4;i++){
		printf("%c",reg_b);
		reg_b= reg_b >>8;
	}
	for(i=0;i<4;i++){
		printf("%c",reg_d);
		reg_d= reg_d >>8;
	}
	for(i=0;i<4;i++){
		printf("%c",reg_c);
		reg_c= reg_c >>8;
	}
}

void show_CPU_name(){
	int reg_a,reg_b,reg_c,reg_d,j;
	unsigned int i;
	for(i=0x80000002;i<0x80000004;i++){
		asm(
			"movl %4,%%eax\n"
			"cpuid\n"
			"movl %%eax,%0\n"
			"movl %%ebx,%1\n"
			"movl %%ecx,%2\n"
			"movl %%edx,%3\n"
			:"=a"(reg_a),"=b"(reg_b),"=c"(reg_c),"=d"(reg_d)
			:"a"(i)
		);
		for(j=0;j<4;j++){
			printf("%c",reg_a);
			reg_a= reg_a >>8;
		}
		for(j=0;j<4;j++){
			printf("%c",reg_b);
			reg_b= reg_b >>8;
		}
		for(j=0;j<4;j++){
			printf("%c",reg_c);
			reg_c= reg_c >>8;
		}
		for(j=0;j<4;j++){
			printf("%c",reg_d);
			reg_d= reg_d >>8;
		}
	}
}
int main()
{
num_of_parameters();
printf("\nVendor:\n");
prod();
printf("\n\nName CPU\n");
show_CPU_name();
printf("\n\n");
}
