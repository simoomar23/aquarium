#include "verification.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


void test_is_id() {
    assert(is_id("N123") == 123);
    assert(is_id("N0") == 0);
    assert(is_id("123") == 0);
    assert(is_id("Nabc") == 0);
    assert(is_id("N123abc") == 0);
    printf("Tous les tests de is_id ont réussi.\n");
}

void test_hello_verification() {
    char tokens1[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "hello" };
    assert(hello_verification(tokens1, 1) == 0);

    char tokens2[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "hello", "in", "as", "N123" };
    assert(hello_verification(tokens2, 4) == 123);

    char tokens3[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "hello", "in", "as", "123" };
    assert(hello_verification(tokens3, 4) == -1);

    char tokens4[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "hello", "world" };
    assert(hello_verification(tokens4, 2) == -1);

    printf("Tous les tests de hello_verification ont réussi.\n");
}

void test_load_show_save_verification() {
    char file[] = "existing_file.txt";
   int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    close(fd);
    char tokens1[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "LOAD", "existing_file.txt" };
    assert(load_show_save_verification(tokens1, 2, file) == 0);

    char tokens2[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "LOAD", "non_existing_file.txt" };
    assert(load_show_save_verification(tokens2, 2, file) == 1);

    char tokens3[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "SAVE", "file.txt" };
    assert(load_show_save_verification(tokens3, 2, file) == 0);

    char tokens4[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "SHOW", "existing_file.txt" };
    assert(load_show_save_verification(tokens4, 2, file) == 0);

    char tokens5[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "SHOW", "different_file.txt" };
    assert(load_show_save_verification(tokens5, 2, file) == 1);

    printf("Tous les tests de load_show_save_verification ont réussi.\n");
}

void test_add_view_verification() {
    int length = 100;
    int width = 100;

    char tokens1[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "ADD", "view", "N1", "10x10+20+20" };
    assert(add_view_verification(tokens1, 4, length, width) == 0);

    char tokens2[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "ADD", "view", "N1", "90x90+20+20" };
    assert(add_view_verification(tokens2, 4, length, width) == 1);

    char tokens3[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "ADD", "view", "N1", "10x10+200+200" };
    assert(add_view_verification(tokens3, 4, length, width) == 1);

    printf("Tous les tests de add_view_verification ont réussi.\n");
}

void test_del_view_verification() {
    char tokens1[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "DEL", "view", "N1" };
    assert(del_view_verification(tokens1, 3) == 0);

    char tokens2[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "DEL", "view", "123" };
    assert(del_view_verification(tokens2, 3) == 1);

    char tokens3[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "DEL", "view" };
    assert(del_view_verification(tokens3, 2) == 1);

    printf("Tous les tests de del_view_verification ont réussi.\n");
}

void test_add_fish_verification() {
    char tokens1[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "ADD", "fish", "at", "10x10,", "20x20,", "east" };
    assert(add_fish_verification(tokens1, 6) == 0);

    char tokens2[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "ADD", "fish", "at", "110x10,", "20x20,", "east" };
    assert(add_fish_verification(tokens2, 6) == 1);

    char tokens3[MAX_TOKENS][MAX_TOKEN_LENGTH] = { "ADD", "fish", "at", "10x10,", "200x20,", "east" };
    assert(add_fish_verification(tokens3, 6) == 1);

    printf("Tous les tests de add_fish_verification ont réussi.\n");
}

int main() {
    test_is_id();
    test_hello_verification();
    test_load_show_save_verification();
    test_add_view_verification();
    test_del_view_verification();
    test_add_fish_verification();
    printf("Tous les tests ont été exécutés avec succès.\n");
    return 0;
}
