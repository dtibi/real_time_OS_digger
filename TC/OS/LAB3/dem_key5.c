/* dem_key5.c - demostrate use of int 16h, AH = 2, including aux byte. */

#include <stdio.h>

typedef struct status_byte
{
  unsigned int right_shift_key : 1;
  unsigned int left_shift_key : 1;
  unsigned int ctrl_key : 1;
  unsigned int alt_key : 1;
  unsigned int scroll_lock_on : 1;
  unsigned int num_lock_on : 1;
  unsigned int caps_lock_on : 1;
  unsigned int insert_mode_on : 1;
}  STATUS_BYTE;

typedef struct aux_byte
{
  unsigned int unused : 3;
  unsigned int hold_on : 1; /* ctrl-num lock */
  unsigned int scroll_lock_depressed : 1;
  unsigned int num_lock_depressed : 1;
  unsigned int caps_lock_depressed : 1;
  unsigned int insert_key_depressed : 1;
}  AUX_BYTE;


void main(void)
 {

   STATUS_BYTE sbyte, sbyte1;
   char *p, *p1;
   AUX_BYTE  abyte;

   asm  {
          MOV AH,2           ; /* BIOS read keyboard status option */
          INT 16h            ; /* BIOS read keyboard status        */
          MOV BYTE PTR sbyte,AL    ; /* Transfer char to program   */
          PUSH ES
          MOV  AX,0
          MOV  ES,AX           /* ES = 0  */
          MOV  AL,ES:[418h]    /* read phisical locations 417h, 418h */
          MOV  BYTE PTR abyte,AL
          MOV  AL,ES:[417h]
          MOV  BYTE PTR sbyte1,AL
          POP ES
        }

   p = (char *) &sbyte;
   p1 = (char *) &sbyte1;

   printf("\n sbyte = %d, sbyte1 = %d\n", (int) *p, (int) *p1);


   printf("\nKeyboard status:\n\n");
   printf("Right shift: %d,\nLeft shift: %d,\nCtrl key: %d,\nAlt key: %d,\n",
            (unsigned int) sbyte.right_shift_key,
              (unsigned int) sbyte.left_shift_key,
                (unsigned int) sbyte.ctrl_key,
                  (unsigned int) sbyte.alt_key);

   printf("Scroll lock on: %d,\nNum lock on: %d,"
      "\nCaps Lock on: %d,\nInsert mode: %d",
            (unsigned int) sbyte.scroll_lock_on,
              (unsigned int) sbyte.num_lock_on,
                (unsigned int) sbyte.caps_lock_on,
                  (unsigned int) sbyte.insert_mode_on);
   putchar('\n');

   printf("\nAuxilary byte:\n\n");
   printf("Hold on: %d,\nScroll lock dep: %d,\nNum lock dep: %d,"
               "\nCaps Lock dep: %d,\nInsert key dep: %d",
            (unsigned int) abyte.hold_on,
            (unsigned int) abyte.scroll_lock_depressed,
              (unsigned int) abyte.num_lock_depressed,
                (unsigned int) abyte.caps_lock_depressed,
                  (unsigned int) abyte.insert_key_depressed);
   putchar('\n');

 } /* main */
