/**
 * @file eval.h
 * @author CHIKAR Soufiane LEFEBVRE Theo
 * @brief Projet IN406
 */
#ifndef _EVAL_H_
#define _EVAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

/**
 * @brief structure qui represente le type d'un token
 * 
 */
typedef enum token_type_s
{
    OPERATOR,
    CONSTANTE,
    NON,
    PARENTHESE_DROITE,
    PARENTHESE_GAUCHE,
    UNKNOWN
} token_type_t;

/**
 * @brief structure qui represente une liste chainé de token
 * 
 */
typedef struct token_s
{
    char data;
    token_type_t type;
    struct token_s *next;
} token_t;
typedef struct token_s *token_list_t;

/**
 * @brief structure qui represente l'abre de token
 * 
 */
typedef struct token_tree_s
{
    token_t token;
    struct token_tree_s *left;
    struct token_tree_s *parent;
    struct token_tree_s *right;
} token_tree_t;

#endif