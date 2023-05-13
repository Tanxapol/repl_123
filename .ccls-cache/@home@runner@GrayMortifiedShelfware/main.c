#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct path {
  void *code;        // BUILDING_T
  float distance;    // 140
  struct path *next; // S1 -> S5
} PATH_T;

typedef struct building {
  int index;             // 0, 1
  char code[10];         // S4, S5
  char name[100];        // Wissawa Wattana Building, Siam Cement Group Building
  struct building *next; // S4 -> S5, S5 -> NULL
  PATH_T *head_path;     // S4
  PATH_T *tail_path;     // S5
} BUILDING_T;

//count path each building function
int count_path(BUILDING_T *start) {
  int count = 0;
  PATH_T *current = start->head_path;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

void deleteN(char *str) {
  if (str[strlen(str) - 1] == '\n') {
    str[strlen(str) - 1] = '\0';
  }
}

//print data function
void print_data(BUILDING_T *start) {
  BUILDING_T *current = start;
  PATH_T *path = NULL;
  while (current != NULL) {
    printf("%d\n", current->index);
    printf("%s\n", current->code);
    printf("%s\n", current->name);
    printf("%d\n", count_path(current));
    path = current->head_path;
    while (path != NULL) {
      BUILDING_T *temp = (BUILDING_T *)path->code;
      printf("%d\n", temp->code);
      printf("%f\n", path->distance);
      path = path->next;
    }
    printf("%d\n", -1);
    current = current->next;
  }
  printf("%d\n", -1);
}

void add_building_form_file(BUILDING_T *start,BUILDING_T *new_building){
  BUILDING_T *current = start;
  if (start == NULL) {
    start = new_building;
    return;
  }
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_building;
  new_building->next = NULL;
}

// write data to file function
void write_data(BUILDING_T *start) {
    FILE *fp = fopen("data.txt", "w");
    BUILDING_T *current = start;
    PATH_T *path = NULL;
    while (current != NULL) {
        fprintf(fp, "%d\n", current->index);
        fprintf(fp, "%s\n", current->code);
        fprintf(fp, "%s\n", current->name);
        fprintf(fp, "%d\n", count_path(current));
        path = current->head_path;
        while (path != NULL) {
        BUILDING_T *temp = (BUILDING_T *)path->code;
        fprintf(fp, "%s\n", temp->code);
        fprintf(fp, "%f\n", path->distance);
        path = path->next;
        }
        fprintf(fp, "%d\n", -1);
        current = current->next;
    }
    fprintf(fp, "%d\n", -1);
    free(current);
    fclose(fp);
}

/*--------------------------------------------------------------------------*/

BUILDING_T *check_vertex(BUILDING_T *graph, char *code) {
  BUILDING_T *g = graph;
  while (g != NULL) // s5(เพิ่งสร้าง)->s6(เพิ่งสร้าง มีpath  ก่อนหน้า) loop ไม่หยุด
  {
    // printf("%s\n", code);
    if (strcmp(g->code, code) == 0) //เช็คว่าใน vertice มี code ที่เป็น path นี้ยัง
    {
      return g;
    }
    g = g->next;
  }
  return NULL;
}

PATH_T *create_path(BUILDING_T *code, float distance) {
  PATH_T *newNode = (PATH_T *)malloc(sizeof(PATH_T));
  newNode->code = code;
  newNode->distance = distance;
  newNode->next = NULL;
  return newNode;
}

BUILDING_T *add_path(BUILDING_T *new, BUILDING_T *newNode, float distance) {
  PATH_T *new_path = create_path(newNode, distance);
  if (new->head_path == NULL) {
    new_path->next = new->head_path;
    new->head_path = new_path;
    new->tail_path = new_path;
  } else {
    new->tail_path->next = new_path;
    new->tail_path = new->tail_path->next;
  }
  return new;
}
int check_path(BUILDING_T *newNode, char *path_code) {
  PATH_T *check = newNode->head_path;
  BUILDING_T *ptr = NULL;
  if (check != NULL) //มีข้อมูลใน path
  {
    while (check != NULL) {
      ptr = check->code; //เก็บ address ของ building
      if (strcmp(ptr->code, path_code) == 0) // code ใน path ตรงกับ code ที่จะใส่ไหม
      {
        return 1; //มีอยู่ใน path แล้ว
      }
      check = check->next;
    }
    if (check == NULL) //ไล่จนถึงตัวสุดท้ายแล้วยังไม่เจอตัวที่เหมือนกัน
    {
      return 0; //ยังไม่มีอยู่ใน path
    }
  } 
  return 0;
}

// read data from file function
void read_data(BUILDING_T *start){
  FILE *fp = fopen("data.txt", "r");
  BUILDING_T *current = start;
  char temp_index[10], temp_code[10], temp_name[100], temp_code_path[10];
  int count = 0, a = 0;
  float temp_distance = 0;

  // read index 
  while(fgets(temp_index, 10, fp) != NULL){
    deleteN(temp_index);
    if(strcmp(temp_index, "-1") == 0){
      break;
    }
    //read code
    fgets(temp_code, 10, fp);
    deleteN(temp_code);
    //read name
    fgets(temp_name, 100, fp);
    deleteN(temp_name);
    //read path
    fscanf(fp, "%d", &count);

    current = check_vertex(start, temp_code);
    if (current == NULL) {
      BUILDING_T *new = (BUILDING_T *)malloc(sizeof(BUILDING_T));
      new->index = atoi(temp_index);
      strcpy(new->code, temp_code);
      strcpy(new->name, temp_name);
      new->next = NULL;
      add_building_form_file(start, new);
      current = new;
    }else{
      current->index = atoi(temp_index);
      strcpy(current->name, temp_name);
    }   

    //read path
    for (int i = 0; i < count; i++) {
      fscanf(fp, "%s", temp_code_path);

      BUILDING_T *temp2path = check_vertex(start, temp_code_path);
      if (temp2path == NULL) {
        BUILDING_T *new_node = (BUILDING_T *)malloc(sizeof(BUILDING_T));
        strcpy(new_node->code, temp_code_path);
        strcpy(new_node->name, "temp");
        new_node->next = NULL;
        temp2path = new_node;
      }

      if(check_path(current, temp_code_path) == 0){
        //read distance
        fscanf(fp, "%f", &temp_distance);
        
        current = add_path(current, temp2path, temp_distance);
        temp2path = add_path(temp2path, current, temp_distance);

        if(check_vertex(start, temp_code_path) == NULL){
          add_building_form_file(start, temp2path);
        }
      }else{
        current = add_path(current, temp2path, temp_distance);
      }
    }
    scanf("%d", &a);
    print_data(current);
  }
}






int main() { 
  // create data
  BUILDING_T G1, S1, S2, S3, S4, S5, S6, S7;
    PATH_T P01, P21, P22, P23, P11, P12, P13, P31, P32, P33, P34, P41, P42, P43, P51, P52, P53, P54, P61, P62, P71, P72;

    G1.index = 0;
    strcpy(G1.code, "G1");
    strcpy(G1.name, "000");
    G1.next = &S1;
    G1.head_path = &P01;
    G1.tail_path = &P01;

    P01.code = &S2;
    P01.distance = 220;
    P01.next = NULL;

    S1.index = 1;
    strcpy(S1.code, "S1");
    strcpy(S1.name, "111");
    S1.next = &S2;
    S1.head_path = &P11;
    S1.tail_path = &P13;

    P11.code = &S2;
    P11.distance = 65;
    P11.next = &P12;

    P12.code = &S3;
    P12.distance = 120;
    P12.next = &P13;

    P13.code = &S4;
    P13.distance = 140;
    P13.next = NULL;

    S2.index = 2;
    strcpy(S2.code, "S2");
    strcpy(S2.name, "222");
    S2.next = &S3;
    S2.head_path = &P21;
    S2.tail_path = &P23;

    P21.code = &G1;
    P21.distance = 220;
    P21.next = &P22;

    P22.code = &S1;
    P22.distance = 65;
    P22.next = &P23;

    P23.code = &S3;
    P23.distance = 55;
    P23.next = NULL;

    S3.index = 3;
    strcpy(S3.code, "S3");
    strcpy(S3.name, "333");
    S3.next = &S4;
    S3.head_path = &P31;
    S3.tail_path = &P34;

    P31.code = &S1;
    P31.distance = 120;
    P31.next = &P32;

    P32.code = &S2;
    P32.distance = 55;
    P32.next = &P33;

    P33.code = &S4;
    P33.distance = 62;
    P33.next = &P34;

    P34.code = &S5;
    P34.distance = 75;
    P34.next = NULL;

    S4.index = 4;
    strcpy(S4.code, "S4");
    strcpy(S4.name, "444");
    S4.next = &S5;
    S4.head_path = &P41;
    S4.tail_path = &P43;

    P41.code = &S1;
    P41.distance = 140;
    P41.next = &P42;

    P42.code = &S3;
    P42.distance = 62;
    P42.next = &P43;

    P43.code = &S5;
    P43.distance = 15;
    P43.next = NULL;

    S5.index = 5;
    strcpy(S5.code, "S5");
    strcpy(S5.name, "555");
    S5.next = &S6;
    S5.head_path = &P51;
    S5.tail_path = &P54;

    P51.code = &S3;
    P51.distance = 75;
    P51.next = &P52;

    P52.code = &S4;
    P52.distance = 15;
    P52.next = &P53;

    P53.code = &S6;
    P53.distance = 120;
    P53.next = &P54;

    P54.code = &S7;
    P54.distance = 82;
    P54.next = NULL;

    S6.index = 6;
    strcpy(S6.code, "S6");
    strcpy(S6.name, "666");
    S6.next = &S7;
    S6.head_path = &P61;
    S6.tail_path = &P62;

    P61.code = &S5;
    P61.distance = 120;
    P61.next = &P62;

    P62.code = &S7;
    P62.distance = 5;
    P62.next = NULL;

    S7.index = 7;
    strcpy(S7.code, "S7");
    strcpy(S7.name, "777");
    S7.next = NULL;
    S7.head_path = &P71;
    S7.tail_path = &P72;

    P71.code = &S5;
    P71.distance = 82;
    P71.next = &P72;

    P72.code = &S6;
    P72.distance = 5;
    P72.next = NULL;
       

    BUILDING_T *start_graph = &G1;
    BUILDING_T *start = NULL;

    

    // print data
    // print_data(start_graph);


    // // write data to file
    // write_data(start_graph);
    read_data(start);

    // print path each all building

    // printf("%d \n", count_path(&S1));
}

/*
S1
afvd
2
S2
234
S3
453
-1
*/