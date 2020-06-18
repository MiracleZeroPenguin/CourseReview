#include <stdio.h>
#include <stdlib.h>

typedef double ElementType;
#define Infinity 1e8
#define Max_Expr 30   /* max size of expression */

ElementType EvalPostfix( char *expr );

int main()
{
    ElementType v;
    char expr[Max_Expr];
    gets(expr);
    v = EvalPostfix( expr );
    if ( v < Infinity )
        printf("%f\n", v);
    else
        printf("ERROR\n");
    return 0;
}

/* Your function will be put here */
ElementType EvalPostfix(char *expr)
{
	double stack[Max_Expr], p1, p2;
	int len = strlen(expr), i, j, flag = 0, top = -1;
	char *tmp;
	for (i = 0; expr[i]; i++) {
		if (!i) {
			if (expr[0] == '-' && expr[1] == '\0')return Infinity;
			if (!isdigit(expr[0]) && expr[0] != '-')flag = 1;
		}
		else if (expr[i] != '.' && !isdigit(expr[i]))
		{
			flag = 1;
		}
	}
	expr[len] = ' ', expr[len + 1] = '\0';
	if (!flag)
		return atof(expr);
	i = 0;
	while (expr[i] != '\0')
	{
		if (expr[i] == ' ')
		{
			expr[i++] = '\0';
			tmp = expr;
			expr += i;
			i = 0;
			if(!((tmp[0]<='9'&&tmp[0]>='0')||tmp[0]=="."||tmp[0]=='+'||tmp[0]=='-'||tmp[0]=='*'||tmp[0]=='/'))
				return Infinity;
			if ((tmp[0] == '+' || tmp[0] == '-' || tmp[0] == '*' || tmp[0] == '/') && tmp[1] == '\0')
			{
				if (top <1)return Infinity;
				p1 = stack[top];
				top--;
				p2 = stack[top];
				top--;

				switch (tmp[0])
				{
				case '+':stack[++top] = p1 + p2; break;
				case'-':stack[++top] = p2 - p1; break;
				case'*':stack[++top] = p2 * p1; break;
				default:
					break;
				}
				if (tmp[0] == '/')
				{
					if (p1 == 0)
						return Infinity;
					stack[++top] = p2 / p1;
				}
			}
			else
				stack[++top] = atof(tmp);
		}
		else i++;
	}
	if (top ==0)
		return stack[0];
	return Infinity;
}
