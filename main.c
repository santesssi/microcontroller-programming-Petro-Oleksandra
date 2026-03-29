#include "project.h"

// --- Функції керування LED ---
void setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
    RED_Write(r);
    GREEN_Write(g);
    BLUE_Write(b);
}

// --- Налаштування Матриці (Твої оригінальні вказівники) ---
static void (*COLUMN_X_SetDriveMode[3]) (uint8_t mode) = {
    Column_0_SetDriveMode, Column_1_SetDriveMode, Column_2_SetDriveMode
};
static void (*COLUMN_x_Write[3]) (uint8_t value) = {
    Column_0_Write, Column_1_Write, Column_2_Write
};
static uint8 (*ROW_x_Read[4]) () = {
    Row_0_Read, Row_1_Read, Row_2_Read, Row_3_Read
};

static uint8_t keys[4][3];

// --- Логіка Пароля ---
static uint8_t input_count = 0;   
static uint8_t is_error = 0; // 0 - поки все вірно, 1 - була помилка в черзі

void processKey(uint8_t num) {
    input_count++;
    
    // Перевірка: 1-ша кнопка має бути 1, 2-га -> 2, 3-тя -> 3
    if (input_count == 1 && num != 1) is_error = 1;
    if (input_count == 2 && num != 2) is_error = 1;
    if (input_count == 3 && num != 3) is_error = 1;

    // Коли натиснуто 3 кнопки - перевіряємо результат
    if (input_count >= 3) {
        if (is_error == 0) {
            SW_Tx_UART_PutString(" > ACCESS GRANTED (123)\r\n");
            setLEDColor(0, 0, 1); // Синій або Зелений (залежить від анода/катода)
        } else {
            SW_Tx_UART_PutString(" > ACCESS DENIED!\r\n");
            setLEDColor(0, 1, 1); // Червоний
        }
        // Скидання для наступного разу
        input_count = 0;
        is_error = 0;
    }
}

// --- Ініціалізація та Читання ---
static void initMatrix() {
    for(int i = 0; i < 3; i++) COLUMN_X_SetDriveMode[i](Column_0_DM_DIG_HIZ);
}

static void readMatrix() {
    for(int c = 0; c < 3; c++) {
        COLUMN_X_SetDriveMode[c](Column_0_DM_STRONG);
        COLUMN_x_Write[c](0);
        for(int r = 0; r < 4; r++) keys[r][c] = ROW_x_Read[r]();
        COLUMN_X_SetDriveMode[c](Column_0_DM_DIG_HIZ);
    }
}

int main(void) {
    CyGlobalIntEnable;
    SW_Tx_UART_Start();
    CyDelay(50);
    SW_Tx_UART_PutString("\r\nSystem Ready. Enter password:\r\n");

    initMatrix();
    setLEDColor(1, 1, 1); // Все вимкнено

    uint8_t last_state = 0;

    for(;;) {
        readMatrix();

        // Перевірка чи всі кнопки відпущені
        uint8_t all_released = 1;
        for(int r=0; r<4; r++) {
            for(int c=0; c<3; c++) {
                if(keys[r][c] == 0) all_released = 0;
            }
        }

        if (all_released) {
            if (last_state != 0) {
                last_state = 0;
            }
        }

        // Ряд 0: Кнопки 1, 2, 3
        if (keys[0][0] == 0 && last_state != 1) { 
            last_state = 1; SW_Tx_UART_PutString("Key 1\r\n"); processKey(1); 
        }
        if (keys[0][1] == 0 && last_state != 2) { 
            last_state = 2; SW_Tx_UART_PutString("Key 2\r\n"); processKey(2); 
        }
        if (keys[0][2] == 0 && last_state != 3) { 
            last_state = 3; SW_Tx_UART_PutString("Key 3\r\n"); processKey(3); 
        }

        // Ряд 1: Кнопки 4, 5, 6
        if (keys[1][0] == 0 && last_state != 4) { 
            last_state = 4; SW_Tx_UART_PutString("Key 4\r\n"); processKey(4); 
        }
        if (keys[1][1] == 0 && last_state != 5) { 
            last_state = 5; SW_Tx_UART_PutString("Key 5\r\n"); processKey(5); 
        }
        if (keys[1][2] == 0 && last_state != 6) { 
            last_state = 6; SW_Tx_UART_PutString("Key 6\r\n"); processKey(6); 
        }

        // Ряд 2: Кнопки 7, 8, 9
        if (keys[2][0] == 0 && last_state != 7) { 
            last_state = 7; SW_Tx_UART_PutString("Key 7\r\n"); processKey(7); 
        }
        if (keys[2][1] == 0 && last_state != 8) { 
            last_state = 8; SW_Tx_UART_PutString("Key 8\r\n"); processKey(8); 
        }
        if (keys[2][2] == 0 && last_state != 9) { 
            last_state = 9; SW_Tx_UART_PutString("Key 9\r\n"); processKey(9); 
        }

        // Ряд 3: Кнопки *, 0, #
        if (keys[3][0] == 0 && last_state != 10) { 
            last_state = 10; SW_Tx_UART_PutString("Key *\r\n"); processKey(10); 
        }
        if (keys[3][1] == 0 && last_state != 11) { 
            last_state = 11; SW_Tx_UART_PutString("Key 0\r\n"); processKey(0); 
        }
        if (keys[3][2] == 0 && last_state != 12) { 
            last_state = 12; SW_Tx_UART_PutString("Key #\r\n"); processKey(11); 
        }

        CyDelay(20);
    }
}