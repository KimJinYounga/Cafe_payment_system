#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define c_size 50  //�� �ִ��
struct menu {
   char product[30];  //��ǰ�̸�
   int price;  //����
};
struct customer {
   char id[30]; //�޴�����ȣ : �ߺ����� ���̵���� ����
   int password; //��й�ȣ 4�ڸ�
   int mypay; //�� �ܰ� �Է�.
};
typedef struct menu MENU;
typedef struct customer customer;
void first(MENU *menu, customer *c_data); //ù ȭ��
int login(customer *data, MENU *menu); //�α���, �� �ܰ� Ȯ��, ȸ������ �ϴ� �Լ�
void menu_prt(MENU *menu); //�޴���� �Լ�
void choice(MENU *menu, int *total);  //�ֹ����� ���� �Լ�
void mypay_charge(customer *data, MENU *menu, int n, int total);//mypay ����
void pay(customer *data, int total, int n);//mypay - �ֹ����� ����Լ�
void sign_in(customer *data); //ȸ������ϴ� �Լ�
void sign_in_id(char *id1, char *id2); //ȸ����� �� id���
void sign_in_password(int *pass1, int *pass2); //ȸ����� �� password���
void charge(customer *data, MENU *menu); //�ܰ� ���

int main(){
   MENU menu[15]={{"�Ƹ޸�ī��",1500},{"����������",2000},{"�����߶�",2500},{"�ٴҶ������Ʈ",2500},{"īǪġ��",1400},
					{"���ݸ�",3500},{"������",3500},{"���⽦��ũ",3500},{"���̽�Ƽ������",2500},{"������Ű����ũ",3500},
					{"����Ƽ",3800},{"�ڸ�Ƽ",3500},{"����Ƽ",3800},{"ĳ����",4000},{"��ũƼ",3300}};
   customer c_data[c_size] = {{"01049494476",4476,0},{"01073226164",6164,0}};//�� ������ ���� ����ü 50��

   first(menu, c_data); // ù ȭ��

   return 0;
}
void first(MENU *menu, customer *c_data){ //ùȭ��
   int choice1, i, n, total, temp;
   while(1){
      system("cls");
      
      for(temp=0;temp<c_size;temp++){ //data �� ó������ password�� 0�� temp ã��.(�����)
         if((c_data+temp)->password == 0)
            break;
      }
      printf("(1)�޴����� (2)ȸ����� (3)����\n");
      scanf("%d", &choice1);
      if(choice1 == 1){
         n = login(c_data , menu); // n = �α��� �� �迭 ��°.
         printf("%d\n",n);

         menu_prt(menu); //�޴����

         choice(menu, &total); //�޴����� total=�Ѱ��� 

         mypay_charge(c_data,menu, n, total); //��������

         pay(c_data, total, n);// �ֹ����� ���
      }
      else if(choice1 == 2){
         sign_in(c_data+temp);
         first(menu, c_data); //ȸ����� �� �ٽ� ó������
      }
      else
         exit(1);
   }
}
void menu_prt(MENU *menu){ //�޴����
   int i;

   printf("                     <<�޴���>>\n");
   printf("=========================================================================================\n\n");
   printf("	   ��caffeine��    |         ��non-caffeine��      |           ��tea��\n");
   printf("			   |				   |\n");
   for(i=0;i<5;i++){
      printf("%2d.%-15s %6d  |    %2d.%-14s %6d   | %2d.%-15s %6d\n",i+1,menu->product, menu->price,
																	i+6,(menu + 5)->product,(menu + 5)->price,
																	i+11,(menu + 10)->product,(menu + 10)->price);
      menu++;
   }
   printf("\n��Ice: +500��, size(middle: +500��, large: +1000��)\n\n");
   printf("=========================================================================================\n\n");
   puts("");
}
void choice(MENU *menu, int *total){ //�ֹ����� ����
   int i, j, price, count, totalprice=0;
   int choice1, choice2, choice3;

   printf("�޴��� � �����Ͻðڽ��ϱ� : ");
   scanf("%d", &count);//count = ������ �޴��� ����
   printf("===============================================\n");
   for(i=0;i<count;i++){
      printf("�޴��� �����Ͻʽÿ� : ");
      scanf("%d",&choice1);
      printf("Hot(1), Ice(2)�� �����Ͻʽÿ� : ");
      scanf("%d", &choice2);
      printf("������ ����(small(1), middle(2), large(3)) : ");
      scanf("%d", &choice3);

      for(j=0;j<15;j++){
         if(choice1 == (j+1)){
            price = (menu+j)->price; //price=���� ����
            if(choice2 == 2)
               price += 500;//price + ice����(500)

            if(choice3 == 2) price += 500; //middle��
            else if(choice3 == 3) price += 1000; //large��

            totalprice+=price;
            printf("-----------------------------------------------\n");
            printf("            %-15s%d\n",(menu+j)->product, price);
            printf("-----------------------------------------------\n");
         } //���� �ֹ������� ���ڿ��� ������.
      }
   }

   printf("               ����     %10d\n", totalprice);
   *total = totalprice; //������ total�� ��� ���� �Լ��� �ѱ�.
}
int login(customer *data, MENU *menu){ //�α���, �� �ܰ� Ȯ���ϴ� �Լ� choice�� �Ѿ..
   int i,temp,count=0, pass;
   int choice1;
   static int result=0;
   char phone_num[30]; //�޴�����ȣ �Է� ����

   printf("      <login>\n");
   printf("�޴��� ��ȣ�� �Է����ּ���: ");
   printf("___________\b\b\b\b\b\b\b\b\b\b\b");
   fflush(stdin);
   gets(phone_num);

   for(temp=0;temp<c_size;temp++){ //data �� ó������ password�� 0�� temp ã��.(�����)
      if((data+temp)->password == 0)
         break;
   }
   for(i=0;i<c_size;i++){ //i=0���� �� �ִ������ �˻�
      if(!strcmp((data+i)->id,phone_num)){ //id ������
         printf("��� ��ȣ�� �Է����ּ���: ");
         printf("____\b\b\b\b");
         scanf("%d",&pass);
         if((data+i)->password == pass){ //���̵���� �н����� �Է�
            printf("�� pay : %d��\n",(data+i)->mypay);
            count=60; //count=0���� count=60���� �ٲ�.
            
			result = i;
            break;
         }
         else{
            system("cls");
            printf("�߸��� ��ȣ�Դϴ�. �α��� ȭ������ ���ư��ϴ�.\n");
            login(data, menu);
            count=60;

            result = i;
         }
         break;
      }
   }
   if(count==0) //for�� ������ count�� �ٸ����� �ȵ���(id�� passwordƲ����)
   {
      system("cls");
      printf("�߸��� ��ȣ�Դϴ�.\n");
      printf("(1)ȸ����� (2)�ٽ��Է�\n");
      scanf("%d",&choice1);
      if(choice1 == 1){ //ȸ����� ����
         sign_in(data+temp);
         (data+temp)->mypay = 0; //�α��� �迭��°�� �� 0����.
         first(menu,data);
      }
      else if(choice1 == 2){ //�ٽ��Է�
         login(data,menu);
      }
   }
   return result;
}
void sign_in(customer *data){ //ȸ������ϴ� �Լ� ���⼭  data=data �� ������ ����.
   char phone_num1[30], phone_num2[30];
   int pass1, pass2, choice, i;
   printf("      <ȸ�����>\n");
   sign_in_id(phone_num1, phone_num2); //id���,Ȯ�� �Լ�
   strcpy(data->id,phone_num1); //id ����Ϸ�(data(temp)��°��)
   printf("id�� ��ϵǾ����ϴ�.\n");
   sign_in_password(&pass1, &pass2); //password ���, Ȯ�� �Լ�
   data->password = pass1;
   printf("password�� ��ϵǾ����ϴ�.\n");

   system("cls");
   printf("      <��ϵ� ����>\n");
   printf("id = %s\n",data->id);
   printf("password = %d\n",data->password);
   printf("Ȯ���ϼ����� (1)�� �����ּ���: ");
      scanf("%d", &choice);
}
void sign_in_id(char *id1, char *id2){ //ȸ����� �� id���
   fflush(stdin);
   printf("�޴��� ��ȣ�� �Է����ּ���: ");
   printf("___________\b\b\b\b\b\b\b\b\b\b\b");
   gets(id1);
   printf("�ٽ� �ѹ� �Է����ּ���: ");
   printf("___________\b\b\b\b\b\b\b\b\b\b\b");
   gets(id2);
   if(strcmp(id1,id2) != 0){
      system("cls");
      printf("�߸��Է��ϼ̽��ϴ�.\n");
      printf("�ʱ� ȭ������ ���ư��ϴ�.\n");
      printf("      <id ���>\n");
      sign_in_id(id1, id2);
   }

}
void sign_in_password(int *pass1, int *pass2){ //ȸ����� �� password���
   printf("��й�ȣ 4�ڸ��� �Է����ּ���: ____\b\b\b\b");
   scanf("%d", pass1);
   printf("�ٽ� �ѹ� �Է����ּ���: ____\b\b\b\b");
   scanf("%d", pass2);
   if(*pass1 != *pass2){
      system("cls");
      printf("�߸��Է��ϼ̽��ϴ�.\n");
      printf("�ʱ� ȭ������ ���ư��ϴ�.\n");
      printf("      <password ���>\n");
      sign_in_password(pass1, pass2);
   }
}
void mypay_charge(customer *data, MENU *menu, int n, int total){ //mypay ����(n = �Է��� �޴�����ȣ �迭��ġ)
   int choice, choice2, price, charge;
   printf("�� pay : %d, ��ǰ�ݾ� : %d\n", (data+n)->mypay, total);
   printf("�� pay�� �����Ͻðڽ��ϱ�?\n");
   printf("(1)YES (2)NO \n");
   scanf("%d", &choice);
   system("cls");
   if(choice==1){
      printf("      <pay ����>\n");
      printf("�� ��ǰ �ݾ� : %d\n",total);
      printf("���� �ݾ��� �Է����ּ��� : ");
      scanf("%d", &price);
      charge = ((data+n)->mypay) + price;
      printf("�� pay : %d + %d = %d��\n", (data+n)->mypay, price, charge);
      (data+n)->mypay = ((data+n)->mypay) + price;
      printf("Ȯ���ϼ����� (1)�� �����ּ���: ");
      scanf("%d", &choice2);
   }
   else if(choice==2)
      if((data+n)->mypay < total){
         system("cls");
         printf("mypay�� �� �ݾ׺��� ���ڶ��ϴ�...\n");
         mypay_charge(data, menu, n, total);
      }
}
void pay(customer *data, int total, int n){ //mypay - �ֹ����� ����Լ�
   int result = (data+n)->mypay - total;
   int last_result = result + (total*0.05);
   int choice;
   system("cls");
   printf("      <���>\n");
   printf("%d - %d = %d\n",(data+n)->mypay,total,result);
   printf("�� ���� pay = %d + %.0lf(5%%����) = %d\n",result, total*0.05, last_result);
   (data+n)->mypay= last_result;
   printf("Ȯ���ϼ����� (1)�� �����ּ���: ");
   scanf("%d", &choice);
}