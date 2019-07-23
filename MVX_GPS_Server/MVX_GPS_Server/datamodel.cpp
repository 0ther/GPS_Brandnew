#include "pch.h"
#include "datamodel.h"

void DATAMODEL::Refresh() {
  SYSTEMTIME st;
  GetLocalTime(&st);
  int Month = st.wMonth;
  std::string example;
  if (Month < 10) example += "0";
  example += std::to_string(Month);
  std::string mystr;
  std::string dat;
  std::ifstream infile("logusers.txt");

  while (!infile.eof())
  {
    getline(infile, mystr); //������ ������� � ��������
    if (mystr == "") break;
    mystr.erase(0, 12);
    mystr.erase(6, mystr.npos);
    mystr += ".txt";
    std::ifstream infile2(mystr);
    if (!infile2.is_open()) {}
    else {
      while (!infile2.eof()) { //������� ������ ������ � ������ � ��, � ������
        getline(infile2, dat);
        if (dat == "") break;
        std::cout << dat << std::endl;
        dat.erase(0, 32);
        dat.erase(2, 3);
        if (dat != example) {
          infile2.close();
          std::fstream in(mystr, std::ios::out);
          in.close();
        }
        break;
      }
      infile2.close();
    }
  }
  infile.close();
}; //������� ������� ������ ������


void DATAMODEL::Select() {
  std::cout << "Choose which type of selection you want to see" << std::endl;
  std::cout << "1. Top 10 lines of the user database" << std::endl;
  std::cout << "2. Bottom 10 lines" << std::endl;
  std::cout << "3. All the lines (Choice of the craziest ones)" << std::endl;
  int type;
  std::cin >> type;
  Selection((SELECTIONTYPES)--type);
}


void DATAMODEL::Selection(SELECTIONTYPES Type) { //������ �����, ����� ���� ����������� � ������������ ������� string, ����� �� ��������� ������������
  std::ifstream Database;
  Database.open("userDB.txt");
  if (Database.is_open()) {
    switch (Type) {
    case TOP: //������� 10 ����� �����
    {
      std::string templine;
      for (int i = 0; i < 10; i++) {
        getline(Database, templine);
        std::cout << templine << std::endl;
      }
    }
    case BOTTOM: //������ 10 ����� �����
    {
        const int K = 10;
        std::string L[K];
        int size = 0;

        /* ������ ���� ��������� � �������� ������ */
        while (Database.good()) {
          getline(Database, L[size % K]);
          size++;
        }

        /* ��������� ������ ��������� ������� � ��� ������ */
        int start = size > K ? (size % K) : 0;
        int count;
        if (size < K) count = size;
        else count = K;

        /* ������� �������� � ������� ������ */
        for (int i = 0; i < count; i++) {
          std::cout << L[(start + i) % K] << std::endl;
        }
    }
    case ALL: //��� ������ �����
    {
      std::string templine;
      while (Database.good()) {
        getline(Database, templine);
        std::cout << templine << std::endl;
      }
    }
    default:
      break;
    }
    Database.close();
  }
}; //������ ������� �� �������


void DATAMODEL::Visualise(std::string begin, std::string end) {
}; //������������� ������ � �������