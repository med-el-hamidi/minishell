#include "../includes/minishell.h"

void print_ast(t_ast *node, int depth)
{
    if (!node) return;

    for (int i = 0; i < depth; i++)
		printf("  ");
    switch (node->type)
        {
        case AST_CMD:
            printf("COMMAND: ");
            for (int i = 0; node->args && node->args[i]; i++)
                printf("%s ", node->args[i]);
            printf("\n");
            break;
        case AST_PIPE:
            printf("PIPELINE\n");
            break;
        case AST_REDIR:
                        if (node->redir_type == REDIR_INPUT)
                printf("REDIR_IN: <%s>\n", node->redir_file);
                        else if (node->redir_type == REDIR_OUTPUT)
                printf("REDIR_OUT: <%s>\n", node->redir_file);
                        else if (node->redir_type == REDIR_APPEND)
                printf("REDIR_APPEND: <%s>\n", node->redir_file);
                        else if (node->redir_type == REDIR_HEREDOC)
                printf("REDIR_HEREDOC: <%s>\n", node->redir_file);
                        else if (node->redir_type == REDIR_NONE)
                printf("REDIR_NONE: <%s>\n", node->redir_file);
            break;
    }

    if (node->left)
        print_ast(node->left, depth + 1);
    if (node->right)
        print_ast(node->right, depth + 1);
}
