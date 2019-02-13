#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 250

typedef struct node
{
	char *data; // payload
	struct node *parent; // pointer to parent
	struct node *left; // pointer to left child
	struct node *right; // pointer to right child
} Node;

typedef struct stack
{
	float array[MAX]; // stores array of floats for 6. calculation option
	int top; // stores the top of the array index
} Stack;

typedef struct varlist
{
	char *names[MAX]; // names of the variables
	float *values[MAX]; // the values
	int top; // because top is shared between the two vars, they are locked to top
} VarList;

Node *newNode(char *newData)
{
	Node *node = malloc(sizeof(Node) * 1); // malloc
	node->data = NULL;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	if (newData != NULL)
	{
		node->data = malloc(sizeof(char) * strlen(newData) + 1);
		strcpy(node->data, newData);
	}
	return node;
}

Stack *newStack()
{
	Stack *stack = calloc(1, sizeof(Stack));
	return stack;
}

VarList *newVarList()
{
	VarList *newList = calloc(1, sizeof(VarList));
	for (int i = 0; i < MAX; i++)
	{
		newList->names[i] = calloc(MAX, sizeof(char));
		newList->values[i] = calloc(MAX, sizeof(float));
	}
	newList->top = -1;
	return newList;
}

void freeStack(Stack *stack)
{
	free(stack);
	return;
}

void freeNode(Node *toFree)
{
	if (toFree->data != NULL)
		free(toFree->data);
	free(toFree);
	return;
}

void freeVarList(VarList *toFree)
{
	for (int i = 0; i < MAX; i++)
	{
		free(toFree->names[i]);
		free(toFree->values[i]);
	}
	free(toFree);
	return;
}

void freeTree(Node *head)
{
	if (head != NULL)
	{
		freeTree(head->left);
		freeTree(head->right);
		freeNode(head);
	}
	return;
}

void printNode(Node *toPrint)
{
	if (toPrint != NULL && toPrint->data != NULL)
	{
		printf("%s", toPrint->data);
	}
	return;
}

void appendLeft(Node *node, char *newData)
{
	if (node != NULL)
	{
		node->left = newNode(newData);
		node->left->parent = node;
	}
	return;
}

void appendRight(Node *node, char *newData)
{
	if (node != NULL)
	{
		node->right = newNode(newData);
		node->right->parent = node;
	}
	return;
}

void prePrint(Node *head)
{
	if (head != NULL)
	{
		printNode(head); printf(" ");
		prePrint(head->left);
		prePrint(head->right);
	}
	return;
}

void inPrint(Node *head)
{
	if (head != NULL)
	{
		if (head->left != NULL)
		{
			if (head->left->left == NULL || head->left->right == NULL)
				inPrint(head->left);
			else
			{
				printf("(");
				inPrint(head->left);
				printf(")");
			}
		}
		printNode(head);
		if (head->right != NULL)
		{
			if (head->right->left == NULL || head->right->right == NULL)
				inPrint(head->right);
			else
			{
				printf("(");
				inPrint(head->right);
				printf(")");
			}
		}
	}
	return;
}

void postPrint(Node *head)
{
	if (head != NULL)
	{
		postPrint(head->left);
		postPrint(head->right);
		printNode(head);
		printf(" ");
	}
	return;
}

int next(char *string, char *token, char *nextString)
{
	char buffer[MAX];
	char tempToken[MAX];
	int result, offset = 0, inputLength = strlen(string);
	switch (string[0])
	{
		case '+': case '-':
		case '*': case '/':
			result = 1;			//RESULT 1 OPERATOR
			offset = 1;
			tempToken[0] = string[0];
			tempToken[1] = '\0';
			strcpy(token, tempToken);
			break;
		case '(':
			result = 2;			//RESULT 2 LEFT BRACKET
			offset = 1;
			strcpy(token, "(");
			break;
		case ')':
			result = 3;			//RESULT 3 RIGHT BRACKET
			offset = 1;
			strcpy(token, ")");
			break;
		case 'x':
			result = 4;			//RESULT 4 VARIABLE OPERAND
			offset = 1;
			for (int i = 1; isdigit(string[i]); i++)
				offset++;
			for (int i = 0; i < offset; i++)
				tempToken[i] = string[i];
			tempToken[offset] = '\0';
			strcpy(token, tempToken);
			break;
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			result = 5;			//RESULT 5 NUMERICAL OPERAND
			offset = 4;
			for (int i = 0; i < offset; i++)
				tempToken[i] = string[i];
			tempToken[offset] = '\0';
			strcpy(token, tempToken);
			break;
	}
	for (int i = offset; i < inputLength; i++)
	{
		buffer[i - offset] = string[i];
	}
	buffer[inputLength - offset] = '\0';
	if (strcmp("", string) == 0)
	{
		strcpy(token, "");
		result = -1;
	}
	strcpy(nextString, buffer);
	return result;
}

int updateList(VarList *list, char *whatVar, char *newValue)
{
	// check if it already exists
	// if it doesn't, create it and initialize->0.0
	// if it does, update the value

	int existence = 0;
	int index = -1;
	float newBoi = atof(newValue);
	for (int i = 0; i <= list->top; i++)
	{
		if (strcmp(whatVar, list->names[i]) == 0)
		{
			existence = 1;
			index = i;
		}
	}

	if (existence == 0)
	{
		list->top = list->top + 1;
		strcpy(list->names[list->top], whatVar);
		*(list->values[list->top]) = 0.0;
		return 0;
	}

	else if (existence == 1)
	{
		*(list->values[index]) = newBoi;
	}
	return 1;
}

float getVariableValue(VarList *list, char *whatVar)
{
	for (int i = 0; i <= list->top; i++)
	{
		if (strcmp(whatVar, list->names[i]) == 0)
		{
			return *(list->values)[i];
		}
	}
	printf("Variable not found\n");
	return 0.0;
}

void parse(Node *tree, char *string, VarList *list)
{
	char *nextString = malloc(sizeof(char) * MAX);
	char *token = malloc(sizeof(char) * MAX);
	int result = next(string, token, nextString);

	switch (result)
	{
		case 1: // OPERATOR
			tree->data = malloc(sizeof(char) * strlen(token) + 1);
			strcpy(tree->data, token);
			parse(tree, nextString, list);
			break;
		case 2: // L BRACKET
			if (tree->left == NULL)
			{
				appendLeft(tree, NULL);
				parse(tree->left, nextString, list);
			} else {
				appendRight(tree, NULL);
				parse(tree->right, nextString, list);
			}
			break;
		case 3: // R BRACKET
			if (tree->parent != NULL)
				parse(tree->parent, nextString, list);
			break;
		case 4: // VARIABLE OP
			if (tree->left == NULL)
			{
				appendLeft(tree, token);
			} else {
				appendRight(tree, token);
			}
			updateList(list, token, "0.0");
			parse(tree, nextString, list);
			break;
		case 5: // NUMERICAL OP
			if (tree->left == NULL)
			{
				appendLeft(tree, token);
			} else {
				appendRight(tree, token);
			}
			parse(tree, nextString, list);
			break;
	}

	free(nextString);
	free(token);
	return;
}

void varReplace(Node *tree, char *toReplace, char* newData)
{
	if (tree != NULL)
	{
		if (strcmp(tree->data, toReplace) == 0)
		{
			tree->data = realloc(tree->data, strlen(newData) + 1);
			strcpy(tree->data, newData);
		}
		if (tree->left != NULL)
			varReplace (tree->left, toReplace, newData);

		if (tree->right != NULL)
			varReplace (tree->right, toReplace, newData);
	}
	return;
}

void push(Stack *stack, float newValue)
{
	stack->array[++stack->top] = newValue;
	return;
}

float pop(Stack *stack)
{
	return stack->array[stack->top--];
}


void postFix(Node *head, char *equation)
{
	if (head != NULL)
	{
		postFix(head->left, equation);
		postFix(head->right, equation);
		if (head->data != NULL)
		{
			if (strcmp("", equation) == 0)
			{
				strcat(equation, head->data);
			}
			else
			{
				strcat(equation, ",");
				strcat(equation, head->data);
			}
		}
	}
	return;
}

int nodeIsZero(Node *node)
{
	if (node == NULL) return 0;
	if (strcmp(node->data, "0") == 0)
		return 1;
	if (strcmp(node->data, "0.0") == 0)
		return 1;
	if (strcmp(node->data, "0.00") == 0)
		return 1;
	return 0;
}

int nodeIsMult(Node *node)
{
	if (node == NULL) return 0;
	if (strcmp(node->data, "*") == 0)
		return 1;
	return 0;
}

int nodeIsSub(Node *node)
{
	if (node == NULL) return 0;
	if (strcmp(node->data, "-") == 0)
		return 1;
	return 0;
}

void divisionByZero(Node *tree, int *flag)
{
	if (tree == NULL) return;
	if (strcmp(tree->data, "/") == 0)
	{
		if (nodeIsZero(tree->right) == 1)
		{
			(*flag) = 1;
		}
		
		if (nodeIsMult(tree->right) == 1)
		{
			if (tree->right->left != NULL)
			{
				if (nodeIsZero(tree->right->left) == 1)
				{
					(*flag) = 1;
				}	
			}

			if (tree->right->right != NULL)
			{
				if (nodeIsZero(tree->right->right) == 1)
				{
					(*flag) = 1;
				}
			}
		}
		
		if (nodeIsSub(tree->right) == 1)
		{
			if (tree->right->left != NULL && tree->right->right != NULL)
			{
				if (strcmp(tree->right->left->data,
							tree->right->right->data) == 0)
				{
					(*flag) = 1;
				}
			}
		}
	}
	divisionByZero(tree->left, flag);
	divisionByZero(tree->right, flag);
	return;
}

void updateVariables(VarList *list, Node *tree)
{
	if (tree == NULL) return;
	for (int i = 0; i <= list->top; i++)
	{
		if (strcmp(list->names[i], tree->data) == 0)
		{
			if (*list->values[i] == 0.00)
			{
				if (tree->data != NULL)
				{
					tree->data = realloc(tree->data, 5);
					strcpy(tree->data, "0.00");
				}
			}
		}
	}
	if (tree->left != NULL)
		updateVariables(list, tree->left);
	if (tree->right != NULL)
		updateVariables(list, tree->right);
	return;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: ./q1 <expression>\n");
		return 1;
	}
	
	char choice = 0;	
	char *equation = argv[1];
	char *userInputVar = calloc(MAX, sizeof(char));
	char *userInputRes = calloc(MAX, sizeof(char));
	int *flag = malloc(sizeof(int));
	(*flag) = 0;

	VarList *list = newVarList();

	Node *tree = newNode(NULL);
	parse(tree, equation, list);

	Node *shadowTree = newNode(NULL);
	VarList *shadowList = newVarList();
	parse(shadowTree, equation, shadowList);

	while (choice != '7')
	{
		divisionByZero(shadowTree->left, flag);
		printf("\n1. Display\n");
		printf("2. Preorder\n");
		printf("3. Inorder\n");
		printf("4. Postorder\n");
		printf("5. Update\n");
		printf("6. Calculate\n");
		printf("7. Exit\n");
		while((choice = getchar()) == '\n' || choice == '\r');
		switch(choice)
		{
			case '1': 
				printf("\nTrees are hard to draw\n\n");
				for (int i = 0; i <= list->top; i++)
				{
					printf("%s:%.1f\n", list->names[i], *list->values[i]);
				}
				break;
			case '2':
				printf("\nPreorder:\n");
				prePrint(tree);
				printf("\n");
				break;
			case '3':
				printf("\nInorder: \n");
				inPrint(tree);
				printf("\n");
				break;
			case '4':
				printf("\nPostorder: \n");
				postPrint(tree);
				printf("\n");
				break;
			case '5':
				printf("\nWhich variable would you like to change? ");
				fgetc(stdin); //TODO This doesn't help
				fgets(userInputVar, MAX, stdin);
				userInputVar[strcspn(userInputVar, "\r\n")] = 0;
				printf("Please enter the new designation for this variable: ");
				fgets(userInputRes, MAX, stdin);
				userInputRes[strcspn(userInputRes, "\r\n")] = 0;
				if (updateList(list, userInputVar, userInputRes) == 0)
					printf("\nVariable %s doesn't exist\n", userInputVar);
				varReplace(shadowTree->left, userInputVar, userInputRes);
				break;
			case '6':
				updateVariables(list, shadowTree->left);
				divisionByZero(shadowTree->left, flag);
				if ((*flag) == 1)
				{
					printf("\nDivision by zero detected. Halting.\n");
					break;
				}
				printf("\nCalculating...\n");
				Stack *stack = newStack();
				char *eq = calloc(MAX, sizeof(char));
				postFix(tree, eq);
				float var1, var2;
				int check = 0;

				char *tmp = strtok(eq, ",");
				while (tmp != NULL)
				{	
					if (check != 0) {tmp = strtok(NULL, ",");}
					if (check == 0) { check = 1; }
					if (tmp != NULL)
					{
						switch (tmp[0])
						{
							case 'x': // var
								push(stack, getVariableValue(list, tmp));
								break;
							case '0': case '1': case '2':
							case '3': case '4': case '5':
							case '6': case '7': case '8':
							case '9': // numOperand
								push(stack, atof(tmp));
								break;
							case '+': // addition
								var1 = pop(stack);
								var2 = pop(stack);
								push(stack, var2 + var1);
								break;
							case '-': // subtraction
								var1 = pop(stack);
								var2 = pop(stack);
								push(stack, var2 - var1);
								break;
							case '*': // multiplication
								var1 = pop(stack);
								var2 = pop(stack);
								push(stack, var2 * var1);
								break;
							case '/': // division
								var1 = pop(stack);
								var2 = pop(stack);
								push(stack, var2 / var1);
								break;
						}
					}
				}
				printf("Result: %f\n", pop(stack));

				free(eq);
				freeStack(stack);
				break;
			case '7':
				printf("Exit\n");
				break;
			default:
				printf("Please enter a number between 1 - 7\n");
				break;
		}
	}
	
	free(userInputVar);
	free(userInputRes);
	freeVarList(list);
	freeVarList(shadowList);
	freeTree(tree);
	freeTree(shadowTree);
	free(flag);

	return 0;
}
