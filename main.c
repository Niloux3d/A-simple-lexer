#define _CRT_SECURE_NO_WARNINGS
#include "scanner.h"

static void run(const char* source) {
    // ��ʼ���ʷ�������
    initScanner(source);

    int line = -1;
    // ��ӡToken, ����TOKEN_EOFΪֹ
    for (;;) {
        Token token = scanToken();

        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        }
        else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}

static void repl() {
    // ���û��������û�ÿ����һ�д��룬����һ�д��룬����������
    // repl��"read evaluate print loop"����д
    char line[1024];
    while (true) {
        printf("input something:\n");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }
        run(line);
    }
}

static char* readFile(const char* path) {
    // �û������ļ������������ļ������ݶ����ڴ棬����ĩβ���'\0'
    // ע��: ����Ӧ��ʹ�ö�̬�ڴ���䣬���Ӧ������ȷ���ļ��Ĵ�С��
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: open file '%s' failed\n", path);
        exit(1);
    }

    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);

    char* buffer = malloc(sizeof(fileSize + 1));
    if (buffer == NULL) {
        printf("Error: malloc failed in readFile\n");
        exit(1);
    }
    int n = fread(buffer, 1, fileSize, fp);
    buffer[n] = '\0';

    fclose(fp);
    return buffer;
}

static void runFile(const char* path) {
    // ����'.c'�ļ�:�û������ļ��������������ļ�������������
    char* source = readFile(path);

    run(source);
    free(source);
}

int main(int argc, const char* argv[]) {
    if (argc == 1) {
        // ./scanner û�в���,����뽻��ʽ����
        repl();
    }
    else if (argc == 2) {
        // ./scanner file �����һ������,����������ļ�
        runFile(argv[1]);
    }
    else {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}