#include "arg_parser.h"
#include <stdio.h>

/**
 * @brief 解析输入字符串，按空白字符分割成参数列表
 *
 * 将输入字符串按空格、换行符、制表符等空白字符分割成多个 token。
 * 连续多个空白字符会被视为单个分隔符，跳过空 token。
 *
 * @param input 待解析的输入字符串
 * @param size 输出参数，返回实际解析出的 token 数量
 * @return char** 动态分配的 token 数组，末尾以 NULL 结尾
 *         调用者负责使用后释放：先释放每个 token，再释放数组本身
 *         返回 NULL 表示解析失败
 */
char **arg_pars(const char *input, size_t *size) {
    int input_len = strlen(input);
    char **tokens;
    tokens = malloc(20 * sizeof(char *));
    int tokens_index = 0;
    char *token;
    int left = 0, right = 0;

    /**
     * @brief 双指针扫描字符串
     *
     * left 指向当前 token 的起始位置
     * right 持续向后扫描，遇到空白字符时切割
     */
    while (right < input_len) {
        if (tokens_index >= 20)
            break;

        if (input[right] == ' ' || input[right] == '\n' ||
            input[right] == '\t') {
            if (left < right) {
                token = malloc(right - left + 1);
                int z = 0;
                for (int i = left; i < right; i++, z++) {
                    token[z] = input[i];
                }
                token[z] = '\0';
                tokens[tokens_index] = token;
                tokens_index++;
            }
            right++;
            left = right;
        } else {
            right++;
        }
    }

    /**
     * @brief 处理最后一个 token
     *
     * 循环结束后，若 right 位置还有未处理的字符，
     * 说明最后一个 token 以字符串结尾结束（非空白字符）
     */
    if (tokens_index < 20 && left < right) {
        token = malloc(right - left + 1);
        int z = 0;
        for (int i = left; i < right; i++, z++) {
            token[z] = input[i];
        }
        token[z] = '\0';
        tokens[tokens_index] = token;
        tokens_index++;
    }

    *size = tokens_index;
    return tokens;
}

/**
 * @brief 命令行参数解析器测试程序
 *
 * 从标准输入读取每一行，使用 arg_pars() 解析并打印所有 token。
 * 支持循环输入，Ctrl+D (Unix) 或 Ctrl+Z (Windows) 退出。
 */
int main() {
    char *input = NULL;
    size_t size = 0;
    size_t token_size;

    while (getline(&input, &size, stdin) != EOF) {
        char **tokens = arg_pars(input, &token_size);
        if (tokens == NULL)
            continue;

        printf("token size : %zu\n", token_size);
        for (int i = 0; i < token_size; i++) {
            printf("token %d : %s\n", i, tokens[i]);
        }

        for (size_t i = 0; i < token_size; i++) {
            free(tokens[i]);
        }
        free(tokens);
        free(input);
        input = NULL;
    }

    return 0;
}
