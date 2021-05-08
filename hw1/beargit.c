#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size> (incl.
 *     NULL character) from file <filename> and store it into <str>. Note that <str>
 *     needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE *findex = fopen(".beargit/.index", "w");
  fclose(findex);

  write_string_to_file(".beargit/.prev",
                       "0000000000000000000000000000000000000000");

  return 0;
}

/* beargit add <filename>
 *
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char *filename) {
  FILE *findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}

/* beargit rm <filename>
 *
 * See "Step 2" in the homework 1 spec.
 *
 */

int beargit_rm(const char *filename) {
  FILE *findex = fopen(".beargit/.index", "r");
  FILE *fnewindex = fopen(".beargit/.newindex", "w");
  int matches_found = 0;

  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      matches_found++;
    } else {
      fprintf(fnewindex, "%s\n", line);
    }
  }

  if (matches_found == 0) {
    fprintf(stderr, "ERROR: File %s not tracked\n", filename);
    fs_rm(".beargit/.newindex");
    fclose(findex);
    fclose(fnewindex);
    return 1;
  }

  fclose(findex);
  fclose(fnewindex);
  fs_mv(".beargit/.newindex", ".beargit/.index");
  return 0;
}

const char *go_bears = "GO BEARS!";

int compare(const char *str1, const char *str2) {
  while (*str1 != '\0' && *str2 != '\0') {
    if (*str1 != *str2)
      return 0;
    str1++;
    str2++;
  }
  return (*str2 == '\0');
}

int is_commit_msg_ok(const char *msg) {
  const char *string_two = go_bears;
  while (*msg != '\0') {
    if (*msg == *string_two && compare(msg, string_two))
      return 1;
    msg++;
  }
  return 0;
}

char next_digit(char *digit) {
  switch (*digit) {
  case '1':
    return '6';
  case '6':
    return 'c';
  case 'c':
    return '1';
  default:
    return 1;
  }
}

char *next_commit_id(char *commit_id) {
  if (compare(commit_id, "0000000000000000000000000000000000000000")) {
    commit_id = "1111111111111111111111111111111111111111";
  } else {
    char change;
    char *commit_id_cursor = commit_id;
    do {
      change = next_digit(commit_id_cursor);
      *commit_id_cursor = change;
      commit_id_cursor++;
    } while (change == '1');
  }
  return commit_id;
}

int beargit_commit(const char *msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }
  char commit_id[COMMIT_ID_SIZE];
  FILE *findex = fopen(".beargit/.prev", "r");
  fgets(commit_id, COMMIT_ID_SIZE, findex);
  strtok(commit_id, "\n");
  char *new_commit_id = next_commit_id(commit_id);

  char new_dir[10 + COMMIT_ID_SIZE];
  strcpy(new_dir, ".beargit/");
  strcat(new_dir, new_commit_id);
  fs_mkdir(new_dir);
  strcat(new_dir, "/");

  char file_path[10 + FILENAME_SIZE + COMMIT_ID_SIZE];
  strcpy(file_path, new_dir);
  strcat(file_path, ".index");
  fs_cp(".beargit/.index", file_path);

  strcpy(file_path, new_dir);
  strcat(file_path, ".prev");
  fs_cp(".beargit/.prev", file_path);

  findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];

  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    strcpy(file_path, new_dir);
    strcat(file_path, line);
    fs_cp(line, file_path);
  }
  fclose(findex);

  findex = fopen(".beargit/.prev", "w");
  fprintf(findex, "%s\n", new_commit_id);
  fclose(findex);

  strcpy(file_path, new_dir);
  strcat(file_path, ".msg");
  findex = fopen(file_path, "w");
  fprintf(findex, "%s", msg);
  fclose(findex);

  return 0;
}

int beargit_status() {
  int number_of_files = 0;
  FILE *findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];

  fprintf(stdout, "Tracked files:\n\n");
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    fprintf(stdout, "  %s\n", line);
    number_of_files++;
  }

  fprintf(stdout, "\n%d files total\n", number_of_files);
  fclose(findex);

  return 0;
}

int beargit_log() {
  char *beargit_dir = ".beargit/";
  char *path_to_prev = ".beargit/.prev";
  char latest_commit[COMMIT_ID_SIZE];
  FILE *findex = fopen(path_to_prev, "r");
  fgets(latest_commit, COMMIT_ID_SIZE, findex);
  strtok(latest_commit, "\n");
  if (compare(latest_commit, "0000000000000000000000000000000000000000")) {
    fclose(findex);
    fprintf(stderr, "ERROR: There are no commits!\n");
    return 1;
  }
  char msg[MSG_SIZE];
  char commit_path[10 + COMMIT_ID_SIZE];
  char msg_path[15 + COMMIT_ID_SIZE];
  char prev_path[15 + COMMIT_ID_SIZE];
  do {
    fprintf(stdout, "\ncommit %s\n", latest_commit);
    strcpy(commit_path, beargit_dir);
    strcat(commit_path, latest_commit);
    strcpy(msg_path, commit_path);
    strcat(msg_path, "/.msg");
    findex = fopen(msg_path, "r");
    fgets(msg, MSG_SIZE, findex);
    strtok(msg, "\n");
    fprintf(stdout, "    %s\n", msg);
    strcpy(prev_path, commit_path);
    strcat(prev_path, "/.prev");
    findex = fopen(prev_path, "r");
    fgets(latest_commit, COMMIT_ID_SIZE, findex);
    strtok(latest_commit, "\n");
  } while (compare(latest_commit, "0000000000000000000000000000000000000000") ==
           0);
  fclose(findex);
  fprintf(stdout, "\n");
  return 0;
}
