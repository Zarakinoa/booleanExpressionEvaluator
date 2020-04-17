/**
 * @file eval.c
 * @author CHIKAR Soufiane LEFEBVRE Theo
 * @brief Projet IN406
 */
#include "../inc/eval.h"

char stack[20];
int top = -1;

/**
 * @brief retourne le type d'un token
 * 
 * @param data 
 * @return token_type_t 
 */
token_type_t get_token_type(char data)
{
    switch (data)
    {
    case '(':
        return PARENTHESE_GAUCHE;
    case ')':
        return PARENTHESE_DROITE;
    case '+':
        return OPERATOR;
    case '1':
        return CONSTANTE;
    case '0':
        return CONSTANTE;
    case '*':
        return OPERATOR;
    case '=':
        return OPERATOR;
    case '>':
        return OPERATOR;
    case '!':
        return NON;
    default:
        return UNKNOWN;
    }
}

/**
 * @brief cree une liste chainee avec un seul element data
 * 
 * @param data 
 * @param type 
 * @param next 
 * @return token_list_t 
 */
token_list_t init_token_list(char data, token_type_t type)
{
    token_list_t new_token_list = (token_list_t)malloc(sizeof(token_t));
    if (new_token_list == NULL)
    {
        perror("\033[31;1mrrror creating a new token list !\033[0m\n");
        exit(EXIT_FAILURE);
    }
    new_token_list->data = data;
    new_token_list->type = type;
    new_token_list->next = NULL;
    return new_token_list;
}

/**
 * @brief ajoute un element data a la liste chainee head
 * 
 * @param head 
 * @param data 
 * @param type 
 * @return token_list_t 
 */
token_list_t append_token_list(token_list_t head, char data, token_type_t type)
{
    token_list_t cursor = head;
    /* On cherche la derniere valeur de la liste */
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }
    /* On rajoute la nouvelle valeur */
    token_list_t new_token = init_token_list(data, type);
    cursor->next = new_token;
    return head;
} 

/**
 * @brief libere la liste chainee head
 * 
 * @param head 
 */
void free_token_list(token_list_t head)
{
    token_list_t cursor, tmp;
    if (head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while (cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}

/**
 * @brief vérifie la liste de token, retourne 1 si elle est correcte sinon 0 
 * 
 * @param head 
 * @return int 
 */
int check_token_list(token_list_t head)
{
    int NB_LEFT_PARENTHESE = 0;
    int NB_RIGHT_PARENTHESE = 0;
    int NB_OPE_SUC = 0;
    token_list_t cursor = head;
    while (cursor != NULL)
    {
        /* On verifie le nombre de parenthese */
        if (cursor->type == PARENTHESE_GAUCHE)
            NB_LEFT_PARENTHESE += 1;

        if (cursor->type == PARENTHESE_DROITE)
            NB_RIGHT_PARENTHESE += 1;

        /* On verifie s'il n'y a pas d'erreur de syntaxe, double operateur par                                  
     * exemple */
        if (cursor->next != NULL)
        {
            if (cursor->type == OPERATOR && cursor->next->type == cursor->type)
                NB_OPE_SUC += 1;
            if (cursor->type == CONSTANTE && cursor->next->type == cursor->type)
                NB_OPE_SUC += 1;
            if (cursor->type == NON && cursor->next->type == OPERATOR)
                NB_OPE_SUC += 1;
            if (cursor->type == NON && cursor->next->type == PARENTHESE_DROITE)
                NB_OPE_SUC += 1;
            if (cursor->type == PARENTHESE_GAUCHE && cursor->next->type == OPERATOR)
                NB_OPE_SUC += 1;
        }
        else if (cursor->type == OPERATOR && cursor->next == NULL)
        {
            NB_OPE_SUC += 1;
        }
        cursor = cursor->next;
    }
    if (NB_OPE_SUC == 0 && NB_LEFT_PARENTHESE == NB_RIGHT_PARENTHESE)
    {
        return 1;
    }
    else
        return 0;
}

/**
 * @brief affiche la liste chainee
 * 
 * @param list 
 */
void print_token_list(token_list_t list)
{
    token_list_t current = list;
    printf("liste de tokens : ");
    while (current != NULL)
    {
        printf("\x1b[32m%c\x1b[0m -> ", current->data);
        current = current->next;
    }
    printf("\x1b[32mNULL\x1b[0m\n");
}

/**
 * @brief transforme une expression en une liste de tokens
 * 
 * @param expression 
 * @return token_list_t 
 */
token_list_t expression_to_token_list(char *expression)
{
    token_list_t token_list = NULL;
    int len = strlen(expression);
    int i = 1;

    /* On Stock notre premier token, parenthése ouvrante ou nombre */
    if (get_token_type(expression[i]) != UNKNOWN)
        token_list = init_token_list(expression[0], get_token_type(expression[0]));

    /* Puis on ajoute les autres token a la suite */
    while (i < len)
    {
        if (get_token_type(expression[i]) != UNKNOWN)
            token_list = append_token_list(token_list, expression[i], get_token_type(expression[i]));
        i++;
    }
    return token_list;
}

/**
 * @brief conjonction entre x et y
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool et(bool x, bool y)
{
    return x && y;
}

/**
 * @brief disjonction entre x et y
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool ou(bool x, bool y)
{
    return x || y;
}

/**
 * @brief x est une condition suffisante pour y, qui, elle, est une condition nécessaire pour x.
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool imp(bool x, bool y)
{
    return !x | y;
}

/**
 * @brief verifie que x est egale a y
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool eqv(bool x, bool y)
{
    return x == y;
}

token_tree_t *new_node(token_t token)
{
    token_tree_t *tmp = malloc(sizeof(token_tree_t));

    tmp->token = token;
    tmp->left = tmp->right = NULL;

    return tmp;
}

/**
 * @brief insertion du noeud token dans l'arbre root
 * 
 * @param root 
 * @param token 
 * @return token_tree_t* 
 */
token_tree_t *insert_token_tree(token_tree_t *root, token_t token)
{
    if (root == NULL)
    {
        root = new_node(token);
    }
    else
    {
        token_tree_t *cursor = root;
        while (cursor != NULL)
        {
            if (cursor->token.type == OPERATOR || cursor->token.type == NON)
            {
                if (cursor->right == NULL)
                {
                    cursor->right = new_node(token);
                    return root;
                }
                else if (cursor->left == NULL)
                {
                    cursor->left = new_node(token);
                    return root;
                }
                else
                {
                    if (cursor->left->token.type == OPERATOR || cursor->left->token.type == NON)
                    {
                        cursor = cursor->left;
                        continue;
                    }
                    else if (cursor->right->token.type == OPERATOR || cursor->right->token.type == NON)
                    {
                        cursor = cursor->right;
                        continue;
                    }
                    else
                    {
                        cursor = cursor->parent->right;
                        continue;
                    }
                }
            }
            else
            {
                perror("Error, only nodes who hold operators can have children.");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/**
 * @brief libere recusivement l'arbre root
 * 
 * @param root 
 */
void free_tree(token_tree_t *root)
{
    if (root != NULL)
    {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

/**
 * @brief affiche l'arbre avec un decalage d
 * 
 * @param root 
 * @param c 
 * @param d 
 */
void print_tree(token_tree_t *root, char *c, int d)
{
    if (root)
    {
        for (int i = 0; i < d; i++)
        {
            printf(" ");
        }
        printf("%s(%c)\n", c, root->token.data);
        print_tree(root->right, "Droite", d + 1);
        print_tree(root->left, "Gauche", d + 1);
    }
}

/**
 * @brief fonction pour avoir un format .dot de l'abre root
 * 
 * @param fd 
 * @param root 
 */
void _print_tree_dot_(FILE *fd, token_tree_t *root)
{
    if (root && root->right)
    {
        if (root->left)
        {
            fprintf(fd, "\"%c\" -- ", root->token.data);

            _print_tree_dot_(fd, root->left);
        }

        fprintf(fd, "\"%c\" -- ", root->token.data);

        _print_tree_dot_(fd, root->right);
    }
    else
        fprintf(fd, "\"%c\";\n", root->token.data);
}

/**
 * @brief genere l'arbre dot dans le descripteur fd
 * 
 * @param fd 
 * @param root 
 */
void print_tree_dot(FILE *fd, token_tree_t *root)
{
    fprintf(fd, "graph G1 {\n");
    _print_tree_dot_(fd, root);
    fprintf(fd, "}\n");
}

/**
 * @brief ajoute un élément sur la pile
 * 
 * @param x 
 */
void push(char x)
{
    stack[++top] = x;
}

/**
 * @brief enlève un élément de la pile et le renvoie
 * 
 * @return char 
 */
char pop()
{
    if (top == -1)
        return -1;
    else
        return stack[top--];
}

/**
 * @brief priorité des operateurs NOT > AND > XOR > OR
 * https://en.cppreference.com/w/c/language/operator_precedence
 * @param x 
 * @return int 
 */
int priority(char x)
{
    if (x == '(')
        return 0;
    if (x == '+')
        return 1;
    if (x == '*')
        return 2;
    if (x == '=' || x == '>')
        return 3;
    if (x == '!')
        return 4;
    return -1;
}

/**
 * @brief ajoute token forme postfixe
 * 
 * @param s 
 * @param c 
 */
void append(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

/**
 * @brief converti l'expression en postfixe
 * 
 * @param expression 
 */
void print_postfix(char *infixe, char *postfixe)
{
    char x;
    while (*infixe != '\0')
    {
        if (isalnum(*infixe))
        {
            append(postfixe, *infixe);
        }
        else if (*infixe == '(')
            push(*infixe);
        else if (*infixe == ')')
        {
            while ((x = pop()) != '(')
            {
                append(postfixe, x);
            }
        }
        else
        {
            while (priority(stack[top]) >= priority(*infixe))
            {
                x = pop();
                append(postfixe, x);
            }
            push(*infixe);
        }
        infixe++;
    }
    while (top != -1)
    {
        x = pop();
        append(postfixe, x);
    }
}

/**
 * @brief inverse la liste de token
 * https://fr.wikipedia.org/wiki/Notation_polonaise_inverse
 * @param head 
 * @return token_list_t 
 */
token_list_t reverse_token_list(token_list_t head)
{
    token_list_t prev = NULL;
    token_list_t cursor = head;
    token_list_t next;
    while (cursor != NULL)
    {
        next = cursor->next;
        cursor->next = prev;
        prev = cursor;
        cursor = next;
    }
    return prev;
}
/**
 * @brief permet de retourner le resultat d'un arbre
 * 
 * @param treenode 
 * @return int 
 */
bool evaluate_tree(token_tree_t *treenode)
{
    bool x, y;

    if (treenode->token.type == OPERATOR)
    {
        x = evaluate_tree(treenode->left);
        y = evaluate_tree(treenode->right);

        if (treenode->token.data == '*')
            return et(x, y);
        else if (treenode->token.data == '+')
            return ou(x, y);
        else if (treenode->token.data == '=')
            return eqv(x, y);
        else if (treenode->token.data == '>')
            return imp(x, y);
        else if (treenode->token.data == '!')
            return !evaluate_tree(treenode);
    }
    else
    {
        return atoi(&treenode->token.data);
    }
}

/**
 * @brief reverse string
 * 
 * @param str 
 * @return char* 
 */
char *strrev(char *str)
{
    char *p1, *p2;

    if (!str || !*str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

/**
 * @brief fonction main
 * 
 * @param argc nombre d'arguments 
 * @param argv les arguments du programme
 * @return int fin du programme
 */
int main(int argc, char **argv)
{
    /* variables */
    token_list_t liste_infixe = NULL;
    token_list_t liste_postfixe = NULL;
    token_list_t liste_prefixe = NULL;

    token_tree_t *arbre = NULL;

    char *infixe;
    char *postfixe = malloc(sizeof(char));
    char *prefixe;
    FILE *fichier;

    printf("Projet IN406 - Evaluation expression booleennes\n\n");

    /* verife de l'argument */
    if (argc != 2)
    {
        perror("\033[31;1mnombre d'arguments invalide !\033[0m\n");
        exit(EXIT_FAILURE);
    }
    infixe = argv[1];
    printf("expression infixe : %s\n", infixe);
    liste_infixe = expression_to_token_list(infixe);

    /* verfie la liste de token */
    if (check_token_list(liste_infixe) == 1)
    {
        print_token_list(liste_infixe);
    }
    else
    {
        printf("\033[31;1mexpression incorrecte !\033[0m\n");
        exit(EXIT_FAILURE);
    }

    printf("\n\n");

    /* convertion de l'expression */
    print_postfix(infixe, postfixe);
    printf("expression postfixe (Reverse Polish) : %s\n", postfixe);
    liste_postfixe = expression_to_token_list(postfixe);
    print_token_list(liste_postfixe);

    printf("\n\n");

    prefixe = strrev(postfixe);
    printf("expression prefixe (Polish) : %s\n", prefixe);
    liste_prefixe = reverse_token_list(liste_postfixe);
    print_token_list(liste_prefixe);

    printf("\n\n");

    /* rendu de l'abre a partir de la racine*/
    printf("arbre binaire : \n");
    /* on insere les tokens */
    while (liste_prefixe != NULL)
    {
        arbre = insert_token_tree(arbre, *liste_prefixe);
        liste_prefixe = liste_prefixe->next;
    }

    print_tree(arbre, "Racine", 0);
    fichier = fopen("out.dot", "wb");
    print_tree_dot(fichier, arbre);

    printf("\n\n");

    printf("resultat de %s est %d \n", infixe, evaluate_tree(arbre));

    /* liberation de la memoire */
    free(postfixe);
    free_tree(arbre);
    free_token_list(liste_infixe);
    free_token_list(liste_prefixe);
    free_token_list(liste_postfixe);
    return 0;
}
