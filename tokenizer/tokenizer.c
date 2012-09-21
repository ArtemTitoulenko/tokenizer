/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEBUG 1

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenT_ {
  char * data;
  struct TokenT_ * next;
};

typedef struct TokenT_ Token;

struct TokenizerT_ {
  char *  target_string; /* the start string to be analyzed */
  Token * head;          /* linked list of tokens to test against */
};

typedef struct TokenizerT_ TokenizerT;


/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
  TokenizerT * tmp_tokenizer = malloc(sizeof(TokenizerT));
  int i = 0, j = 1, len = (int) strlen(separators);

  tmp_tokenizer->head = NULL; // INITIALIZE, OTHERWISE WE HAVE UNDEFINED BEHAVIOR
  tmp_tokenizer->target_string = strdup(ts);
  assert(tmp_tokenizer->target_string != NULL);

  for (; j <= len; i++, j++) {
    Token *tmp_token = malloc(sizeof(Token));
    assert(tmp_token != NULL);

    if (strcmp(&separators[i], "\\") == 0) {
      if (strncmp(&separators[i], "\x00", 4) && !strncmp(&separators[i], "\xff", 4)) {
        // this means that the string is in the range of the hex codes
      }
    }

    tmp_token->data = strndup(&separators[i],1);
    assert(tmp_token->data != NULL);

    tmp_token->next = tmp_tokenizer->head; // point it to the list
    tmp_tokenizer->head = tmp_token; // set it as the new entry point in the linked list
    if (DEBUG) printf("token: %s\n", tmp_tokenizer->head->data);
  }

  return tmp_tokenizer;
}

void print_tokens(TokenizerT * tk) {
  Token * target = tk->head;

  for (; target != NULL; target = target->next) {
    puts(target->data);
  }

  target = NULL;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void free_next_in_list(Token *t) {
  if (t->next != NULL) {
    free_next_in_list(t->next);
  }
  free(t);
}

void TKDestroy(TokenizerT *tk) {
  free_next_in_list(tk->head);
  free(tk->target_string);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
  Token * target = tk->head;
  char * i = tk->target_string,
       * lowest = tk->target_string + strlen(tk->target_string),
       * chunk = NULL;

  for (; target != NULL; target = target->next) {
    i = strstr(tk->target_string, target->data);

    if (i != NULL && i < lowest) {
      lowest = i;
    }
  };

  if (strcmp(tk->target_string, "\0") == 0) {
    return NULL;
  } else if (lowest == tk->target_string) {
    tk->target_string += 1;
    return TKGetNextToken(tk);
  } else {
    if (strlen(tk->target_string) == 0) {
      return NULL;
    }

    if (lowest == tk->target_string && strlen(tk->target_string) > 0) {
      chunk = strdup(tk->target_string);
      tk->target_string += strlen(tk->target_string);
    } else {
      chunk = strndup(tk->target_string, lowest - tk->target_string);
      tk->target_string = lowest + 1;
    }

    return chunk;
  }
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */


int main(int argc, char **argv) {
  TokenizerT * tokenizer;
  char * chunk;

  if(argc != 3) {
    puts("usage: tokenizer \"<tokens>\" \"<target string>\"");
    return 0;
  }

  puts("start->");

  tokenizer = TKCreate(argv[1], argv[2]);

  while (chunk = TKGetNextToken(tokenizer), chunk != NULL) {
    puts(chunk);
    free(chunk);
  }

  return 0;

  TKDestroy(tokenizer);
  puts("real");
  free(tokenizer);

  return 0;
}
