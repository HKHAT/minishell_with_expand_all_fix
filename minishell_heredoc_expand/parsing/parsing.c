/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elchakir <elchakir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 21:36:21 by yozainan          #+#    #+#             */
/*   Updated: 2024/07/26 23:20:14 by elchakir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int check_ambiguous_unexpected_tokens(Token *tokens,int i)
{

        if(((tokens[i].type == TOKEN_HERE_DOC && tokens[i + 1].type != TOKEN_WORD) || 
            (tokens[i].type == TOKEN_REDIRECT_IN && tokens[i + 1].type != TOKEN_WORD ) || 
            (tokens[i].type == TOKEN_REDIRECT_OUT && tokens[i + 1].type != TOKEN_WORD) ||
            (tokens[i].type == TOKEN_APPEND_OUT && tokens[i + 1].type != TOKEN_WORD)) && (tokens[i + 1].type != TOKEN_AMBIGUOUS))
        {
            return (printf("Syntax error: near unexpected token 2 '%s'\n", tokens[i].value),1);
        }   
        return 0;
}



bool is_token_invalid(Token *tokens, int i, int nb_tokens);
bool is_double_token_error(Token *tokens, int i);
bool is_initial_token_error(Token *tokens, int i);
bool is_final_token_error(Token *tokens, int i, int nb_tokens);
bool is_invalid_sequence(Token *tokens, int i);

int check_syntaxe(Token *tokens, int nb_tokens) {
   int i;
   i = 0;
   while( i < nb_tokens && tokens[i].type != TOKEN_END) 
    {
        if(tokens[i].type == NOT_DEFINE)
            return 1;
        if (tokens[i].type == TOKEN_OR ||tokens[i].type == TOKEN_AND || tokens[i].type == TOKEN_LPR 
        || tokens[i].type == TOKEN_BACKGROUND ||tokens[i].type == TOKEN_RPR)
            return printf("Syntax error: undefined token '%s'\n", tokens[i].value), 1;
        if(tokens[i].type == TOKEN_PIPE && tokens[i - 1].type != TOKEN_AMBIGUOUS && tokens[i + 1].type == TOKEN_END)
            return printf("Syntax error: undefined token 111 '%s'\n", tokens[i].value), 1;
        if(check_ambiguous_unexpected_tokens(tokens,i))
            return 1;
        if (is_double_token_error(tokens, i))
            return 1;
        if (is_initial_token_error(tokens, i))
            return 1;
        if (is_final_token_error(tokens, i, nb_tokens))
            return 1;
        if (is_invalid_sequence(tokens, i))
            return 1;
        i++;
    }
    return 0;
}

bool is_double_token_error(Token *tokens, int i) {
    if (tokens[i].type == tokens[i + 1].type && tokens[i].type != TOKEN_WORD && tokens[i].type != TOKEN_END) {
        return printf("Syntax error: unexpected token '%s'\n", tokens[i].value), true;
    }
    return false;
}

bool is_initial_token_error(Token *tokens, int i) {
    if (i == 0 && (tokens[i].type == TOKEN_AND || tokens[i].type == TOKEN_OR 
    || tokens[i].type == TOKEN_PIPE || tokens[i].type == TOKEN_BACKGROUND)) 
    {
        return printf("Syntax error: unexpected token '%s'\n", tokens[i].value), true;
    }
    return false;
}

bool is_final_token_error(Token *tokens, int i, int nb_tokens) {
    if (i == nb_tokens - 1 && (tokens[i].type == TOKEN_AND || tokens[i].type == TOKEN_OR 
    || tokens[i].type == TOKEN_PIPE || tokens[i].type == TOKEN_BACKGROUND)) {
        return printf("Syntax error: unexpected token '%s'\n", tokens[i].value), true;
    }
    return false;
}

bool is_invalid_sequence(Token *tokens, int i) {
    if ((tokens[i].type == TOKEN_AND && tokens[i + 1].type == TOKEN_AND) ||
        (tokens[i].type == TOKEN_OR && tokens[i + 1].type == TOKEN_OR) ||
        (tokens[i].type == TOKEN_BACKGROUND && tokens[i + 1].type == TOKEN_BACKGROUND)) {
        return printf("Syntax error: unexpected token '%s'\n", tokens[i].value), true;
    }
    return false;
}
// Helper function to check for unexpected tokens

// int check_syntaxe(Token *tokens,int nb_tokens)
// {
//     int i;
//     int j;
//     i = 0;

//     while (i < nb_tokens && tokens[i].type != TOKEN_END)
//     {
//         if(tokens[i].type == NOT_DEFINE)
//                 return 1;
//     if(tokens[i].type == TOKEN_LPR || tokens[i + 1].type == TOKEN_LPR 
//     ||tokens[i + 1].type == TOKEN_RPR || tokens[i + 1].type == TOKEN_RPR  || tokens[i].type == TOKEN_BACKGROUND||tokens[i].type == TOKEN_AND || tokens[i].type == TOKEN_OR)
//             return (printf("Syntax error: unexpected token   '%s'\n", tokens[i].value), 1);
//     if((j = check_ambiguous_unexpected_tokens(tokens,i)) >= 1 
//         || check_unexpected_tokens_pipe_and_background(tokens,&i) == 1)
//         {
//             return 1;  
//     }
//     else if(i == 0 && tokens[i].type != TOKEN_WORD && tokens[i].type != TOKEN_HERE_DOC 
//         && tokens[i].type != TOKEN_REDIRECT_IN && tokens[i].type != TOKEN_REDIRECT_OUT && tokens[i].type != TOKEN_APPEND_OUT)
//         {
//              return (printf("Syntax error: unexpected token 88'%s'\n", tokens[i].value), 1);
//         }
//         else if (tokens[i].type != TOKEN_END && tokens[i].type == tokens[i + 1].type 
//                  && tokens[i].type != TOKEN_WORD)
//         {
//             return (printf("Syntax error: unexpected token 6 '%s'\n", tokens[i].value), 1);
//         }
//         else if ((tokens[i].type == TOKEN_BACKGROUND || tokens[i].type == TOKEN_PIPE || tokens[i].type == TOKEN_AND) 
//                  && (tokens[i + 1].type == TOKEN_END || tokens[i + 1].type == tokens[i].type))
//         {
//             return (printf("Syntax error: unexpected token 7 '%s'\n", tokens[i].value), 1);
//         }
// 	else if(tokens[i].type != TOKEN_WORD && tokens[i].type != TOKEN_AMBIGUOUS && (tokens[i + 1].type == TOKEN_PIPE 
// 				|| tokens[i + 1].type == TOKEN_END || tokens[i + 1].type == TOKEN_OR))
//         {
// 		return (printf("Syntax error: unexpected token 78 '%s'\n", tokens[i].value), 1);
//         }
//     else if(i > 1 && tokens[i - 1].type != TOKEN_WORD  &&tokens[i - 1].type != TOKEN_AMBIGUOUS &&  tokens[i - 1].type != TOKEN_LPR &&tokens[i - 1].type != TOKEN_RPR
//      && (tokens[i].type == TOKEN_PIPE 
// 				|| tokens[i].type == TOKEN_END || tokens[i].type == TOKEN_OR))
//         {
//             return (printf("Syntax error: unexpected token 98 '%s'\n", tokens[i].value), 1);
//         }
//         i++;
//     }
//     return 0;
// }

bool is_operator(Token token) 
{
    return token.type == TOKEN_PIPE || token.type == TOKEN_AND || token.type == TOKEN_OR;
}

void process_operator(Token *tokens, int *i, Command **current, int *argc, Command **head) 
{
    finalize_command(current, argc);
    add_command_to_list(head, *current);
    *current = create_command();
    if (!*current) return; // Handle memory allocation failure
    (*current)->type = &tokens[*i].type;
}

bool is_redirection(Token token) 
{
    return token.type == TOKEN_REDIRECT_IN || token.type == TOKEN_REDIRECT_OUT || 
           token.type == TOKEN_APPEND_OUT || token.type == TOKEN_HERE_DOC;
}

bool is_parenthesis(Token *tokens, int i) 
{
    return (tokens[i].type == TOKEN_LPR && tokens[i + 1].type == TOKEN_WORD) ||
           (tokens[i].type == TOKEN_RPR && tokens[i - 1].type == TOKEN_WORD && i > 1);
}

void handle_syntax_error() {
    write(2, "minishell: syntax error near unexpected token newline\n", 55);
}

void process_token(Token *tokens, t_value *value, Command **current, Command **head) 
{
    if (tokens[value->i].type == TOKEN_WORD) {
        handle_word(tokens, &value->i, current, &value->argc);
    } else if (is_operator(tokens[value->i])) {
        process_operator(tokens, &value->i, current, &value->argc, head);
    } else if (is_redirection(tokens[value->i])) {
        handle_redirection(tokens, &value->i, current);
        value->flag++;
    } else if (is_parenthesis(tokens, value->i)) {
        value->flag++;
    } else if (tokens[value->i].type != TOKEN_AMBIGUOUS) {
        handle_syntax_error();
    }
    value->i++;
}



Command* parse(Token *tokens) 
{
    Command *head;
    Command *current;
    t_value value;
    value.flag = 0;
    value.i = 0;
    value.argc = 0;
    head = NULL;
    current = create_command();


    if (!current) return NULL; // Handle memory allocation failure

    while (tokens[value.i].type != TOKEN_END) 
    {
        process_token(tokens, &value, &current, &head);
    }

    if (current && (value.argc > 0 || value.flag > 0)) 
    {
        finalize_command(&current, &value.argc);
        add_command_to_list(&head, current);
    }

    return head;
}

// Command* parse(Token *tokens) 
// {
//     Command *head = NULL;
//     Command *current;
//     int flag;
//     int argc;
//     int i;
//     current = create_command();
//     if (!current) return NULL; // Handle memory allocation failure
//     flag = 0;
//     argc = 0;
//     i = 0;
//     while (tokens[i].type != TOKEN_END) {
//         if (tokens[i].type == TOKEN_WORD) 
//         {
//             handle_word(tokens, &i, &current, &argc);
//         } else if ((tokens[i].type == TOKEN_PIPE || tokens[i].type == TOKEN_AND 
//         || tokens[i].type == TOKEN_OR))
//         {
//             finalize_command(&current, &argc);
//             add_command_to_list(&head, current);
//             current = create_command(); // Start a new command
//             if (!current) return head; // Handle memory allocation failure
//             current->type = &tokens[i].type;
//         }
//         else if (tokens[i].type == TOKEN_REDIRECT_IN || tokens[i].type == TOKEN_REDIRECT_OUT 
//                 || tokens[i].type == TOKEN_APPEND_OUT || tokens[i].type == TOKEN_HERE_DOC) 
//         {
//                 handle_redirection(tokens, &i, &current);
//                 flag++;
//         } else if ((tokens[i].type == TOKEN_LPR && tokens[i + 1].type == TOKEN_WORD )
//         || (tokens[i].type == TOKEN_RPR && tokens[i - 1].type == TOKEN_WORD && i > 1)) {
//             flag++;
//         } else if(tokens[i].type != TOKEN_AMBIGUOUS){
//             write(2, "minishell: syntax error near unexpected token  newline\n", 56);
//             return (NULL);
//         }
//         i++;
//     }
//     if (current && (argc > 0 || flag > 0)) { // Check if there's anything in the current command
//         finalize_command(&current, &argc);
//         add_command_to_list(&head, current);
//     }
//     return head;
// }



int ft_size(char **argv)
{
    int i;

    if (argv == NULL)
        return 0;
    i = 0;
    while(argv[i] != NULL)
        i++;
    return i;
}

