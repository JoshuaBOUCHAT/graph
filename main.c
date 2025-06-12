#include <stdio.h>

int find_max(char *s, int start, int end);
char *longestPalindrome(char *s) {
  int i = 0;
  int max = 0;
  int max_idx = 0;

  while (s[i]) {
    int extend = find_max(s, i, i);
    int temp_max = 1 + (extend << 1);
    if (temp_max > max) {
      max = temp_max;
      max_idx = i - extend;
    }
    if (s[i + 1] != 0 && s[i] == s[i + 1]) {
      extend = find_max(s, i, i + 1);
      temp_max = 2 + (extend << 1);
      if (temp_max > max) {
        max = temp_max;
        max_idx = i - extend;
      }
    }
    i++;
  }
  char *ret = s + max_idx;
  ret[max] = 0;
  return ret;
}

int find_max(char *s, int start, int end) {
  int i = 0;
  char b;
  while (--start >= 0 && (b = s[++end])) {
    if (s[start] == b) {
      i++;
    } else {
      break;
    }
  }
  return i;
}
int main() {
  char s[] = "babab";
  printf("%s", longestPalindrome(s));
}
