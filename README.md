# Binary Expression Tree Parser
This is a command-line program that takes as input a fully-parenthesized, arithmetic expression of binary operators {+, -, \*, /} and converts this expression into a binary expression tree. The inputted expression is a character string without any spaces in it.

The expression inputted to the program can contain floating point numbers of the form Y.YY -- that is -- one digit to the left of the decimal point and two digits to the right.

You are also allowed to include variables of the form x1, x2, etc. These variables may be updated later by the user before the expression evaluates.

However, due to the way command-line arguments are passed, the brackets you send to the command-line must be escaped... So in order to send an expression like (((x1+5.12)\*(x2-7.68))/x3), you would have to send:

Example: ./parser \\(\\(\\(x1+5.12\\)\*\\(x2-7.68\\)\\)/x3\\)

The program has 7 menu options thereafter:
1. Display -- originally this would have drawn the tree, but I could not implement this feature. Alas, it just displays the values of the variables if they are updated by the user.

2. Preorder -- displays the entered equation in prefix notation. For example, the above equation in preorder would be / \* + x1 5.12 - x2 7.68 x3

3. Inorder -- displays the entered equation in infix notation, so it would be displayed as-is (this was a requirement of the assignment and required some internal work -- it wasn't just "re-print the argument".

4. Postorder -- displays the entered equation in reverse Polish notation, or postfix notation. The above equation would read x1 5.12 + x2 7.68 - \* x3 /

5. Update -- allows the user to update variables. This is the menu option that would allow the user to change the x1, x2, and x3 from the original example equation.

6. Calculate -- finally, this takes everything into account and calculates the final result. If the user's variables have not been updated, they default to 0. This program uses error-checking and halts if there is a division by zero though.
