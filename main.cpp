
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;
int rom[1 << 20];
unsigned int pc = 0;
unsigned int regs[32];
bool flag=true;
ofstream out;


enum INSTRUCTIONS {
    LUI,      //U
    AUIPC,    //U  
    JAL,      //J
    JALR,     //I
    BEQ,      //B
    BNE,      //B
    BLT,      //B
    BGE,      //B
    BLTU,     //B
    BGEU,     //B
    LB,       //I
    LH,       //I
    LW,       //I
    LBU,      //I
    LHU,      //I
    SB,       //S
    SH,       //S
    SW,       //S
    ADDI,     //I
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    ADD,      //R
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND
};

void print_thing(int i)
{
    switch (i)
    {
    case 0:
        out << "LUI";
        break;
    case 1:
        out << "AUIPC";
        break;
    case 2:
        out << "JAL";
        break;
    case 3:
        out << "JALR";
        break;
    case 4:
        out << "BEQ";
        break;
    case 5:
        out << "BNE";
        break;
    case 6:
        out << "BLT";
        break;
    case 7:
        out << "BGE";
        break;
    case 8:
        out << "BLTU";
        break;
    case 9:
        out << "BGEU";
        break;
    case 10:
        out << "LB";
        break;
    case 11:
        out << "LH";
        break;
    case 12:
        out << "LW";
        break;
    case 13:
        out << "LBU";
        break;
    case 14:
        out << "LHU";
        break;
    case 15:
        out << "SB";
        break;
    case 16:
        out << "SH";
        break;
    case 17:
        out << "SW";
        break;
    case 18:
        out << "ADDI";
        break;
    case 19:
        out << "SLTI";
        break;
    case 20:
        out << "SLTIU";
        break;
    case 21:
        out << "XORI";
        break;
    case 22:
        out << "ORI";
        break;
    case 23:
        out << "ANDI";
        break;
    case 24:
        out << "SLLI";
        break;
    case 25:
        out << "SRLI";
        break;
    case 26:
        out << "SRAI";
        break;
    case 27:
        out << "ADD";
        break;
    case 28:
        out << "SUB";
        break;
    case 29:
        out << "SLL";
        break;
    case 30:
        out << "SLT";
        break;
    case 31:
        out << "SLTU";
        break;
    case 32:
        out << "XOR";
        break;
    case 33:
        out << "SRL";
        break;
    case 34:
        out << "SRA";
        break;
    case 35:
        out << "OR";
        break;
    case 36:
        out << "AND";
        break;
    }
}
int X_to_D(char* s, int X_len)
{
    char* tmp;
    tmp = new char[X_len];

    int i = 0;
    int result = 0, k = 1;
    for (i = 0;i < X_len;++i)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            tmp[i] = s[i] - '0';
        }
        else
        {
            switch (s[i])
            {
            case'a':case 'A':tmp[i] = 10;
                break;
            case'b':case 'B':tmp[i] = 11;
                break;
            case'c':case 'C':tmp[i] = 12;
                break;
            case'd':case 'D':tmp[i] = 13;
                break;
            case'e':case 'E':tmp[i] = 14;
                break;
            case'f':case 'F':tmp[i] = 15;
                break;
            }
        }
        result += (long long)(tmp[i]) * k;
        k *= 16;
    }

    return result;
}

bool read_in()
{

    static char ch = cin.get();
    if (ch == EOF)
        return false;
    while (ch != EOF)
    {
        char add[8];
        if (ch == '@')
        {
            for (int i = 0;i < 8;++i)
            {
                ch = cin.get();
                add[7 - i] = ch;
            }
            pc = X_to_D(add, 8);
        }

        else if (ch == '\n')
        {
            ch = cin.get();
            continue;
        }
        else if (ch == ' ')
        {
            ch = cin.get();
            continue;
        }
        else
        {
            char ch0 = cin.get();
            char chs[2];
            chs[0] = ch0;
            chs[1] = ch;
            rom[pc] = X_to_D(chs, 2);
            pc += 1;
        }
        ch = cin.get();
    }
    pc = 0;
    if (ch == EOF)
        return false;
    return true;
}

//从模拟内存得到一条指令,转为unsigned int
unsigned int get_inst()
{
    char ch[8];
    ch[1] = rom[pc] / 16 + '0';
    ch[0] = rom[pc] % 16 + '0';
    ch[3] = rom[pc + 1] / 16 + '0';
    ch[2] = rom[pc + 1] % 16 + '0';
    ch[5] = rom[pc + 2] / 16 + '0';
    ch[4] = rom[pc + 2] % 16 + '0';
    ch[7] = rom[pc + 3] / 16 + '0';
    ch[6] = rom[pc + 3] % 16 + '0';
    unsigned int ans = 0;
    for (int i = 0;i < 8;i++) {
        if (ch[i] < 'A') ch[i] = ch[i] - '0';
        else ch[i] = ch[i] - 'A' + 10;
        ans |= ((unsigned int)(ch[i]) << (i << 2));
    }

//    cout << hex << ans << '\t' << dec;
    return ans;

}

struct instruction
{
    unsigned int bin;
    INSTRUCTIONS op;
    char type;
    unsigned int rs1, rs2, rd, imm=0, adress=0;
    instruction(unsigned int num)
    {
        bin = num;
        unsigned int func3 = (num >> 12) % 8;
        unsigned int opcode = num % (1 << 7);
        unsigned int func7 = (num >> 25) % (1 << 7);
        switch (opcode)
        {

        case 0b0110111:
            op = LUI;
            break;
        case 0b1101111:
            op = JAL;
            break;
        case 0b1100111:
            op = JALR;
            break;
        case 0b1100011:
        {
            switch (func3)
            {
            case 0b000:
                op = BEQ;
                break;
            case 0b001:
                op = BNE;
                break;
            case 0b100:
                op = BLT;
                break;
            case 0b101:
                op = BGE;
                break;
            case 0b110:
                op = BLTU;
                break;
            case 0b111:
                op = BGEU;
                break;
            }
            break;
        }
        case 0b0000011:
        {
            switch (func3)
            {
            case 0b000:
                op = LB;
                break;
            case 0b001:
                op = LH;
                break;
            case 0b010:
                op = LW;
                break;
            case 0b100:
                op = LBU;
                break;
            case 0b101:
                op = LHU;
                break;
            }
            break;

        }
        case 0b0100011:
        {
            switch (func3)
            {
            case 0b000:
                op = SB;
                break;
            case 0b001:
                op = SH;
                break;
            case 0b010:
                op = SW;
                break;
            }
            break;

        }
        case 0b0010011:
        {
            switch (func3)
            {
            case 0b000:
                op = ADDI;
                break;
            case 0b010:
                op = SLTI;
                break;
            case 0b011:
                op = SLTIU;
                break;
            case 0b100:
                op = XORI;
                break;
            case 0b110:
                op = ORI;
                break;
            case 0b111:
                op = ANDI;
                break;
            case 0b001:
                op = SLLI;
                break;
            case 0b101:
            {
                if (func7 == 0b0000000)
                {
                    op = SRLI;
                }

                else if (func7 == 0b0100000)
                {
                    op = SRAI;
                }

                break;
            }

            }
            break;

        }
        case 0b0110011:
        {
            switch (func3)
            {
            case 0b000:
            {
                if (func7 == 0b0000000)
                {
                    op = ADD;
                }

                else if (func7 == 0b0100000)
                {
                    op = SUB;
                }

                break;
            }
            case 0b001:
                op = SLL;
                break;
            case 0b010:
                op = SLT;
                break;
            case 0b011:
                op = SLTU;
                break;
            case 0b100:
                op = XOR;
                break;
            case 0b101:
            {
                if (func7 == 0b0000000)
                {
                    op = SRL;
                }

                else if (func7 == 0b0100000)
                {
                    op = SRA;
                }

                break;
            }
            case 0b110:
                op = OR;
                break;
            case 0b111:
                op = AND;
                break;
            }
            break;

        }
        }
        if (op == LUI)type = 'U';
        else if (op == ADDI || op == SLTI || op == SLTIU || op == ANDI || op == ORI || op == XORI || op == SLLI || op == SRLI || op == SRAI || op == JALR)type = 'I';
        else if (op == ADD || op == SLT || op == SLTU || op == AND || op == OR || op == XOR || op == SLL || op == SRL || op == SUB || op == SRA)type = 'R';
        else if (op == JAL)type = 'J';
        else if (op == BEQ || op == BNE || op == BLT || op == BLTU || op == BGE || op == BGEU)type = 'B';
        else if (op == LW || op == LH || op == LHU || op == LB || op == LBU)type = 'I';
        else if (op == SW || op == SH || op == SB)type = 'S';
//        cout <<' ' << op << '\t';
    }

    void decode()
    {
        if (type == 'U')
        {
            rd = (bin >> 7) % 32;
            imm = (bin & 0xFFFFF000UL);
        }
        else if (type == 'I')
        {
            rs1 = (bin >> 15) % 32;
            rd = (bin >> 7) % 32;
            if ((bin >> 31u) == 1u)
                imm |= 0xFFFFF800;
            imm |= (bin >> 20u) & 2047u;
        }
        else if (type == 'R')
        {
            rs1 = (bin >> 15) % 32;
            rs2 = (bin >> 20) % 32;
            rd = (bin >> 7) % 32;
        }
        else if (type == 'J')
        {
            rd = (bin >> 7) % 32;

            if ((bin >> 31u) == 1u)
                imm |= 0xFFF00000UL;
            imm |= bin & 0x000ff000UL;
            imm |= ((bin >> 20u) & 1u) << 11u;
            imm |= ((bin >> 21u) & 1023u) << 1u;
        }
        else if (type == 'B')
        {
            rs1 = (bin >> 15) % 32;
            rs2 = (bin >> 20) % 32;
            if ((bin >> 31u) == 1u)
                imm |= 0xfffff000;
            imm |= ((bin >> 7u) & 1u) << 11u;
            imm |= ((bin >> 25u) & 63u) << 5u;
            imm |= ((bin >> 8u) & 15u) << 1u;
        }
        else if (type == 'S')
        {
            rs1 = (bin >> 15) % 32;
            rs2 = (bin >> 20) % 32;
            if ((bin >> 31u) == 0x1u)
                imm |= 0xfffff800;
            imm |= ((bin >> 25u) & 63u) << 5u;
            imm |= ((bin >> 8u) & 15u) << 1u;
            imm |= (bin >> 7u) & 1u;
        }
    }
    void execute()
    {
        if (rd == 0 && op != JAL && op != JALR&& type!='B' && type != 'S')
            return;
        if (type == 'U')
        {
            if (op == LUI)
            {
                regs[rd] = imm;
            }
        }
        else if (type == 'J')
        {
            if (op == JAL)
            {
                if(rd!=0)
                    regs[rd] = pc + 4;
                pc += (int)imm;
                pc -= 4;
            }
        }
        else if (type == 'I')
        {
            if (op == JALR)
            {
                if(rd!=0)
                    regs[rd] = pc + 4;
                pc = (int)((imm + regs[rs1])>>1)<<1 ;
                pc -= 4;
            }
            else if (op == ADDI)
            {
                regs[rd] = regs[rs1] + imm;
            }
            else if (op == SLTI)
            {
                if ((int)regs[rs1] < (int)imm)
                    regs[rd] = 1;
                else
                    regs[rd] = 0;
            }
            else if (op == SLTIU)
            {
                if ((int)regs[rs1] < (int)imm)
                    regs[rd] = 1;
                else
                    regs[rd] = 0;
            }
            else if (op == ANDI)
            {
                regs[rd] = regs[rs1] & imm;
            }
            else if (op == ORI)
            {
                regs[rd] = regs[rs1] | imm;
            }
            else if (op == XORI)
            {
                regs[rd] = regs[rs1] ^ imm;
            }
            else if (op == SLLI)
            {
                regs[rd] = regs[rs1] << (imm & 31UL);
            }
            else if (op == SRLI)
            {
                regs[rd] = regs[rs1] >> (imm & 31UL);
            }
            else if (op == SRAI)
            {
                regs[rd] = (int)regs[rs1] >> (imm & 31UL);
            }
            else
            {
                adress = regs[rs1] + (int)imm;
            }
        }
        else if (type == 'B')
        {
            if (op == BEQ)
            {
                
                if (regs[rs1] == regs[rs2])
                {
                    pc += imm;
                    pc -= 4;
                }
            }
            else if (op == BNE)
            {
                if (regs[rs1] != regs[rs2])
                {
                    pc += imm;
                    pc -= 4;
                }
            }
            else if (op == BLT)
            {
                if ((int)regs[rs1] < (int)regs[rs2])
                {
                    pc += imm;
                    pc -= 4;
                }
            }
            else if (op == BLTU)
            {
                if (regs[rs1] < regs[rs2])
                {
                    pc += imm;
                    pc -= 4;
                }
            }
            else if (op == BGE)
            {
                if ((int)regs[rs1] >= (int)regs[rs2])
                {
                    pc += imm;
                    pc -= 4;
                }
            }
            else if (op == BGEU)
            {
                if (regs[rs1] >= regs[rs2])
                {
                    pc += imm;
                    pc -= 4;
                }
            }
        }
        else if (type == 'R')
        {
            if (op == ADD)
            {
                regs[rd] = regs[rs1] + regs[rs2];
            }
            else if (op == SUB)
            {
                regs[rd] = regs[rs1] - regs[rs2];
            }
            else if (op == SLL)
            {
                regs[rd] = regs[rs1] << (regs[rs2] & 31UL);
            }
            else if (op == SRL)
            {
                regs[rd] = regs[rs1] >> (regs[rs2] & 31UL);
            }
            else if (op == SRA)
            {
                regs[rd] = ((int)regs[rs1]) >> (regs[rs2] & 31UL);
            }
            else if (op == SLT)
            {
                if ((int)regs[rs1] < (int)regs[rs2])
                    regs[rd] = 1;
                else
                    regs[rd] = 0;
            }
            else if (op == SLTU)
            {
                if (regs[rs1] < regs[rs2])
                    regs[rd] = 1;
                else
                    regs[rd] = 0;
            }
            else if (op == AND)
            {
                regs[rd] = regs[rs1] & regs[rs2];
            }
            else if (op == OR)
            {
                regs[rd] = regs[rs1] | regs[rs2];
            }
            else if (op == XOR)
            {
                regs[rd] = regs[rs1] ^ regs[rs2];
            }
        }
        else if (type == 'S')
        {
            adress = regs[rs1] + (int)imm;
        }
    }
    void memory()
    {
        if (type=='I'&&rd == 0)
            return;
        if (type == 'I')
        {
            if (op == LB)
            {
                char ch[2];
                ch[1] = rom[adress] / 16 + '0';
                ch[0] = rom[adress] % 16 + '0';
                unsigned int res = 0;
                for (int i = 0;i < 2;i++) {
                    if (ch[i] < 'A') ch[i] = ch[i] - '0';
                    else ch[i] = ch[i] - 'A' + 10;
                    res |= ((unsigned int)(ch[i]) << (i << 2));
                }
                regs[rd] = (int)res;
            }
            else if (op == LH)
            {
                char ch[4];
                ch[1] = rom[adress] / 16 + '0';
                ch[0] = rom[adress] % 16 + '0';
                ch[3] = rom[adress + 1] / 16 + '0';
                ch[2] = rom[adress + 1] % 16 + '0';
                unsigned int res = 0;
                for (int i = 0;i < 4;i++) {
                    if (ch[i] < 'A') ch[i] = ch[i] - '0';
                    else ch[i] = ch[i] - 'A' + 10;
                    res |= ((unsigned int)(ch[i]) << (i << 2));
                }
                regs[rd] = (int)res;
            }
            else if (op == LW)
            {
                char ch[8];
                ch[1] = rom[adress] / 16 + '0';
                ch[0] = rom[adress] % 16 + '0';
                ch[3] = rom[adress + 1] / 16 + '0';
                ch[2] = rom[adress + 1] % 16 + '0';
                ch[5] = rom[adress + 2] / 16 + '0';
                ch[4] = rom[adress + 2] % 16 + '0';
                ch[7] = rom[adress + 3] / 16 + '0';
                ch[6] = rom[adress + 3] % 16 + '0';
                unsigned int res = 0;
                for (int i = 0;i < 8;i++) {
                    if (ch[i] < 'A') ch[i] = ch[i] - '0';
                    else ch[i] = ch[i] - 'A' + 10;
                    res |= ((unsigned int)(ch[i]) << (i << 2));
                }
                regs[rd] = (int)res;
            }
            else if (op == LBU)
            {
                char ch[2];
                ch[1] = rom[adress] / 16 + '0';
                ch[0] = rom[adress] % 16 + '0';
                unsigned int res = 0;
                for (int i = 0;i < 2;i++) {
                    if (ch[i] < 'A') ch[i] = ch[i] - '0';
                    else ch[i] = ch[i] - 'A' + 10;
                    res |= ((unsigned int)(ch[i]) << (i << 2));
                }
                regs[rd] = res;
            }
            else if (op == LHU)
            {
                char ch[4];
                ch[1] = rom[adress] / 16 + '0';
                ch[0] = rom[adress] % 16 + '0';
                ch[3] = rom[adress + 1] / 16 + '0';
                ch[2] = rom[adress + 1] % 16 + '0';
                unsigned int res = 0;
                for (int i = 0;i < 4;i++) {
                    if (ch[i] < 'A') ch[i] = ch[i] - '0';
                    else ch[i] = ch[i] - 'A' + 10;
                    res |= ((unsigned int)(ch[i]) << (i << 2));
                }
                regs[rd] = res;
            }
        }
        else if (type == 'S')
        {
            if (op == SB)
            {
                rom[adress] = (regs[rs2] % (1 << 8));
            }
            else if (op == SH)
            {
                rom[adress] = (regs[rs2] % (1 << 8));
                rom[adress + 1] = ((regs[rs2] >> 8) % (1 << 8));
            }
            else if (op == SW)
            {
                rom[adress] = (regs[rs2] % (1 << 8));
                rom[adress + 1] = ((regs[rs2] >> 8) % (1 << 8));
                rom[adress + 2] = ((regs[rs2] >> 16) % (1 << 8));
                rom[adress + 3] = ((regs[rs2] >> 24) % (1 << 8));
            }
        }
    }
};
int main()
{
    ;
    char operation[32];
    for (int i = 0;i < (1 << 20);++i)
        rom[i] = 0;

    out.open("res.txt");
    read_in();
    int tick = 1;
    while(1)
    {
        unsigned int operation = get_inst();
        if (operation == 0x0ff00513)
        {
            cout << (((unsigned int)regs[10]) & 255u );
            break;
        }
        else
        {
            
//            cout << hex << pc << dec ;
            out <<dec << "current tick "<<tick<<'\t';
            regs[0] = 0;
            instruction inst(operation);

            out << hex << operation<<'\t';
            out << hex << pc << '\t';

            inst.decode();

            out << "operation ";
            print_thing(inst.op);
            out << endl;

            inst.execute();
            
            pc += 4;
            inst.memory();

            if (tick == 31855)
            {
//                cout << endl << endl << inst.op << ' ' << inst.rs1 << ' ' <<regs[inst.rs1]<<' '<< inst.rs2<<' '<<regs[inst.rs2]<< ' ' << (int)inst.imm<< ' ';
            }
//            cout << ' ' << (int)regs[10]<<'\t'<<tick<<endl;
            tick++;
        }
        
    }
        



}