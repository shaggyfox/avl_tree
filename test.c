#include <stdlib.h>
#include <stdio.h>
#include "avl-tree.h"

int pr(avl_tree **tpp, void *dat) {
  if (dat) {
    printf("%s\n", dat);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  avl_tree *tree = NULL;
  for (int i = 0; i < 100; ++i) {
    int r = rand()%100;
    int r2 = rand()%2;
    printf("-----------------------------------\n");
    if (r2) {
      printf("ADD %i\n", r);
      avl_insert_integer(&tree, r, NULL);
    } else {
      printf("DEL %i\n", r);
      avl_delete_integer(&tree, r);
    }
    avl_print(tree);
  }

  char *words[] = {
"hanging",
"deep",
"responsible",
"married",
"able",
"strong",
"beautiful",
"quick",
"acid",
"blue",
"living",
"great",
"feeble",
"different",
"tall",
"sudden",
"loose",
"certain",
"female",
"past",
"military",
"important",
"narrow",
"yellow",
"regular",
"new",
"high",
"bright",
"fat",
"smooth",
"shut",
"quiet",
"special",
"green",
"natural",
"slow",
"black",
"late",
"sticky",
"happy",
"early",
"male",
"cheap",
"young",
"serious",
"hard",
"false",
"solid",
"electric",
"violent",
"cut",
"private",
"bitter",
"clean",
"old",
"hollow",
"complex",
"secret",
"medical",
"left",
"first",
"small",
"straight",
"chemical",
"healthy",
"dry",
"like",
"free",
"right",
"physical",
"safe",
"dark",
"general",
"last",
"thick",
"stiff",
"dear",
"material",
"dirty",
"parallel",
"full",
"conscious",
"open",
"sweet",
"political",
"awake",
"boiling",
"good",
"low",
"frequent",
"long",
"wrong",
"short",
"tight",
"broken",
"flat",
"future",
"probable",
"brown",
"second",
"sharp",
"tired",
"public",
"necessary",
"fixed",
"fertile",
"strange",
"rough",
"sad",
"dependent",
"wet",
"red",
"present",
"dead",
"cruel",
"cold",
"simple",
"equal",
"clear",
"bad",
"round",
"warm",
"normal",
"white",
"poor",
"ill",
"thin",
"angry",
"mixed",
"ready",
"true",
"waiting",
"possible",
"wide",
"loud",
"separate",
"automatic",
"elastic",
"chief",
"delicate",
"gray",
"opposite",
"wise",
"common",
"bent",
"same",
"kind",
"foolish",
"complete",
"soft",
NULL
  };

  for(int i = 0; words[i];++i) {
      avl_insert_string(&tree, words[i], words[i]);
  }
  avl_print(tree);
  avl_foreach(tree, &pr);
  return 0;
}

