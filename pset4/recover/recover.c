#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
    BYTE *buffer = malloc(sizeof(BYTE) * 512);
    char *filename = malloc(sizeof(char) * 10);
    int n = 0;
    int end = 0;
    // 輸入參數錯誤
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    //檔案讀取錯誤
    if (file == NULL)
    {
        printf("The forensic image cannot be opened\n");
        return 1;
    }
    while (!end)
    {
        //第一次讀
        if (n == 0)
        {
            fread(buffer, sizeof(BYTE), 512, file);
        }
        //找jpeg檔
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //儲存檔名
            sprintf(filename, "%03i.jpg", n);
            n++;
            FILE *out = fopen(filename, "w");
            //繼續讀下個block直到讀到下個jpeg檔為止
            do
            {
                //寫入jpeg檔
                fwrite(buffer, sizeof(BYTE), 512, out);
                //讀完記憶卡資料end設為1
                if (!fread(buffer, sizeof(BYTE), 512, file))
                {
                    end = 1;
                    break;
                }
            }
            while (!(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0));
            fclose(out);
        }
    }
    fclose(file);
    free(buffer);
    free(filename);
}