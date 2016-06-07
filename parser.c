#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "structures.h"
#include "constants.h"
#include "core_functions.h"

void parse(struct Tree * root, struct Tokens *tokens){

    while(tokens->counter < tokens->length){
        struct Token token = tokens->tokens[tokens->counter];
        if(token.type == 'o'){
            struct Tree * tree = malloc(sizeof(struct Tree));
            tree->type = 'f';
            tree->size = 0;

            tokens->counter++;
            parse(tree, tokens);
            root->children[root->size] = tree;
            root->size++;
        }
        else if(is_a_function(token.type)){
            root->type = token.type;
        }
        else if(token.type == 'b'){ //open bracket
            struct Tree * value = malloc(sizeof(struct Tree));
            value->type = 'a'; // a for array
            value->size = 0;
            struct Value content = {
                .type='a',
            };
            content.data.array = malloc(sizeof(struct Value_array));
            content.data.array->size = 0;
            while(tokens->tokens[tokens->counter].type != 'e'){
                tokens->counter++;
                if(tokens->tokens[tokens->counter].type == 's'){
                    struct Value * val = malloc(sizeof(struct Value));
                    val->type = 's';
                    val->data.str.length = tokens->tokens[tokens->counter].data.length;
                    strcpy(val->data.str.body, tokens->tokens[tokens->counter].data.body);
                    content.data.array->values[content.data.array->size] = val;
                    content.data.array->size++;
                }
                else if(tokens->tokens[tokens->counter].type == 'n'){
                    struct Value * val = malloc(sizeof(struct Value));
                    val->data.ln = atol(tokens->tokens[tokens->counter].data.body);
                    val->type = 'l';
                    content.data.array->values[content.data.array->size] = val;
                    content.data.array->size++;
                }
            }
            value->content = content;
            root->children[root->size] = value;
            root->size++;
        }
        else if(token.type == 'k'){
            // if we are inside a def call this could be name or args
            if(root->type != 'f'){
                // != 'f' means we already set this root node as some kind of function
                struct Tree * value = malloc(sizeof(struct Tree));
                value->type = token.type;
                value->size = 0;
                struct String content = {
                    .length = token.data.length
                };
                strcpy(content.body, token.data.body);
                struct Value val = {
                    .type='s',
                    .data={
                        .str=content
                    }
                };
                value->content = val;

                root->children[root->size] = value;
                root->size++;
            } else {
                root->type = token.type;
                struct String content = {
                    .length = token.data.length
                };
                strcpy(content.body, token.data.body);
                struct Value val = {
                    .type='s',
                    .data={
                        .str=content
                    }
                };
                root->content = val;
            }
        }
        else if(token.type == 's'){
            struct Tree * value = malloc(sizeof(struct Tree));
            value->type = token.type;
            value->size = 0;
            struct String content = {
                .length = token.data.length
            };
            strcpy(content.body, token.data.body);
            struct Value val = {
                .type='s',
                .data={
                    .str=content
                }
            };
            value->content = val;

            root->children[root->size] = value;
            root->size++;
        }
        else if(token.type == 'n'){
            struct Tree * value = malloc(sizeof(struct Tree));
            value->type = token.type;
            value->size = 0;
            long content = atol(token.data.body);
            struct Value val = {
                .type='l',
                .data={
                    .ln=content
                }
            };
            value->content = val;

            root->children[root->size] = value;
            root->size++;
        }
        else if(token.type == 'c'){
            return;
        }
        tokens->counter++;
    }
    return;
}
