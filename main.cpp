#include <iostream>

#define Registers 8
#define Memory 1024

struct memory_unit
{
    bool bit[16];
};

const int bit2int(const memory_unit & a)
{
    int result=0,c=1;
    for (int i=0;i<16;i++)
    {
        result+=c*a.bit[i];
        c=c*2;
    }
    return result;
}

class Computer
{
private:
    memory_unit R[Registers],M[Memory],PC,IR;
    bool NZP[3];
protected:
    memory_unit ALU_plus(memory_unit &A1,memory_unit &A2) //Arithmetic Logic Unit +operation
    {
        memory_unit rslt;
        bool c=0;
        for (int i=0;i<16;i++)
        {
            rslt.bit[i]=A1.bit[i]*A2.bit[i]*c+A1.bit[i]*(!A2.bit[i])*(!c)+(!A1.bit[i])*A2.bit[i]*(!c)+(!A1.bit[i])*(!A2.bit[i])*c;
            c=A1.bit[i]*A2.bit[i]+A1.bit[i]*c+A2.bit[i]*c;
        }
        return rslt;
    }
    
    memory_unit ALU_and(memory_unit &A1,memory_unit &A2)
    {
        memory_unit result;
        for (int i=0;i<16;i++)
            result.bit[i]=A1.bit[i]*A2.bit[i];
        return result;
    }
    
    void PC_incr()
    {
        memory_unit plus1;
        for (int i=1;i<16;i++)
            plus1.bit[i]=0;
        plus1.bit[0]=1;
        PC=ALU_plus(PC,plus1);
    }
private:
    // SIGN EXTENSION FUNCTIONS
    
    memory_unit SEXT5(const memory_unit &a)
    {
        memory_unit result;
        for (int i=0;i<5;i++)
            result.bit[i]=IR.bit[i];
        for (int i=5;i<16;i++)
            result.bit[i]=IR.bit[4];
        return result;
    }
    
    memory_unit SEXT6(const memory_unit &a)
    {
        memory_unit result;
        for (int i=0;i<6;i++)
            result.bit[i]=IR.bit[i];
        for (int i=6;i<16;i++)
            result.bit[i]=IR.bit[5];
        return result;
    }
    
    memory_unit SEXT9(const memory_unit &a)
    {
        memory_unit result;
        for (int i=0;i<9;i++)
            result.bit[i]=IR.bit[i];
        for (int i=9;i<16;i++)
            result.bit[i]=IR.bit[8];
        return result;
    }
    
    memory_unit SEXT11(const memory_unit &a)
    {
        memory_unit result;
        for (int i=0;i<11;i++)
            result.bit[i]=IR.bit[i];
        for (int i=11;i<16;i++)
            result.bit[i]=IR.bit[10];
        return result;
    }
    
    //END OF SIGN EXTENSION
    void add1()
    {
        int r1=IR.bit[0]+2*IR.bit[1]+4*IR.bit[2];
        int r2=IR.bit[6]+2*IR.bit[7]+4*IR.bit[8];
        int dr=IR.bit[9]+2*IR.bit[10]+4*IR.bit[11];
        R[dr]=ALU_plus(R[r1],R[r2]);
        if(R[dr].bit[15]==1)
        {
            NZP[0]=1;
            NZP[1]=0;
            NZP[2]=0;
        }
        else
        {
            for (int i=0;i<15;i++)
                if(R[dr].bit[i]==1)
                {
                    NZP[0]=0;
                    NZP[1]=0;
                    NZP[2]=1;
                    return;
                }
            NZP[0]=0;
            NZP[1]=1;
            NZP[2]=0;
        }
    }
    
    void add2()
    {
        memory_unit s=SEXT5(IR);
        int dr=IR.bit[9]+2*IR.bit[10]+4*IR.bit[11];
        int sr1=IR.bit[6]+IR.bit[7]*2+IR.bit[8]*4;
        R[dr]=ALU_plus(R[sr1],s);
        if(R[dr].bit[15]==1)
        {
            NZP[0]=1;
            NZP[1]=0;
            NZP[2]=0;
        }
        else
        {
            for (int i=0;i<15;i++)
                if(R[dr].bit[i]==1)
                {
                    NZP[0]=0;
                    NZP[1]=0;
                    NZP[2]=1;
                    return;
                }
            NZP[0]=0;
            NZP[1]=1;
            NZP[2]=0;
        }
    }
    
    void and1()
    {
        int r1=IR.bit[0]+2*IR.bit[1]+4*IR.bit[2];
        int r2=IR.bit[6]+2*IR.bit[7]+4*IR.bit[8];
        int dr=IR.bit[9]+2*IR.bit[10]+4*IR.bit[11];
        R[dr]=ALU_and(R[r1],R[r2]);
        if(R[dr].bit[15]==1)
        {
            NZP[0]=1;
            NZP[1]=0;
            NZP[2]=0;
        }
        else
        {
            for (int i=0;i<15;i++)
                if(R[dr].bit[i]==1)
                {
                    NZP[0]=0;
                    NZP[1]=0;
                    NZP[2]=1;
                    return;
                }
            NZP[0]=0;
            NZP[1]=1;
            NZP[2]=0;
        }
    }
    
    void and2()
    {
        int dr=IR.bit[9]+2*IR.bit[10]+4*IR.bit[11];
        int sr1=IR.bit[6]+2*IR.bit[7]+4*IR.bit[8];
        memory_unit s=SEXT5(IR);
        R[dr]=ALU_and(R[sr1],s);
        if(R[dr].bit[15]==1)
        {
            NZP[0]=1;
            NZP[1]=0;
            NZP[2]=0;
        }
        else
        {
            for (int i=0;i<15;i++)
                if(R[dr].bit[i]==1)
                {
                    NZP[0]=0;
                    NZP[1]=0;
                    NZP[2]=1;
                    return;
                }
            NZP[0]=0;
            NZP[1]=1;
            NZP[2]=0;
        }
    }
    
    void br()
    {
        if(NZP[0]*IR.bit[11]+NZP[1]*IR.bit[10]+NZP[2]*IR.bit[9])
        {
            memory_unit s=SEXT9(IR);
            PC=ALU_plus(PC,s);
        }
    }
    
    void jmp()
    {
        int r_store=IR.bit[6]+IR.bit[7]*2+IR.bit[8]*4;
        PC=R[r_store];
    }
    
    void jsr()
    {
        R[7]=PC;
        memory_unit s=SEXT11(IR);
        PC=ALU_plus(PC,s);
    }
    
    void jsrr()
    {
        R[7]=PC;
        PC=R[IR.bit[6]+IR.bit[7]*2+IR.bit[8]*4];
    }
    
    void ret()
    {
        PC=R[7];
    }
    
    void ld()
    {
        int dr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        memory_unit s=SEXT9(IR);
        R[dr]=M[bit2int(ALU_plus(PC,s))];
    }
    
    void ldi()
    {
        int dr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        memory_unit s=SEXT9(IR);
        R[dr]=M[bit2int(M[bit2int(ALU_plus(PC,s))])];
    }
    
    void ldr()
    {
        int dr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        int sr=IR.bit[6]+IR.bit[7]*2+IR.bit[8]*4;
        memory_unit of=SEXT6(IR);
        R[dr]=M[bit2int(ALU_plus(R[sr],of))];
    }
    
    void lea()
    {
        int dr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        memory_unit of=SEXT9(IR);
        R[dr]=ALU_plus(PC,of);
    }
    
    void NoT()
    {
        int dr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        int sr=IR.bit[6]+IR.bit[7]*2+IR.bit[8]*4;
        for (int i=0;i<16;i++)
            R[dr].bit[i]=!R[sr].bit[i];
        if(R[dr].bit[15]==1)
        {
            NZP[0]=1;
            NZP[1]=0;
            NZP[2]=0;
        }
        else
        {
            for (int i=0;i<15;i++)
                if(R[dr].bit[i]==1)
                {
                    NZP[0]=0;
                    NZP[1]=0;
                    NZP[2]=1;
                    return;
                }
            NZP[0]=0;
            NZP[1]=1;
            NZP[2]=0;
        }
    }
    
    void st()
    {
        int sr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        memory_unit of=SEXT9(IR);
        M[bit2int(ALU_plus(PC,of))]=R[sr];
    }
    
    void sti()
    {
        int sr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        memory_unit of=SEXT9(IR);
        M[bit2int(M[bit2int(ALU_plus(PC,of))])]=R[sr];
    }
    
    void str()
    {
        int sr=IR.bit[9]+IR.bit[10]*2+IR.bit[11]*4;
        int dr=IR.bit[6]+IR.bit[7]*2+IR.bit[8]*4;
        memory_unit of=SEXT6(IR);
        M[bit2int(ALU_plus(R[dr],of))]=R[sr];
    }
    
    void fetch()
    {
        IR=M[bit2int(PC)];
    }
    void decode()
    {
        PC_incr();
        
        if (IR.bit[15]==0 && IR.bit[14]==0 && IR.bit[13]==0 && IR.bit[12]==1 && IR.bit[5]==0)
            add1();
        else if(IR.bit[15]==0 && IR.bit[14]==0 && IR.bit[13]==0 && IR.bit[12]==1 && IR.bit[5]==1)
            add2();
        else if(IR.bit[15]==0 && IR.bit[14]==1 && IR.bit[13]==0 && IR.bit[12]==1 && IR.bit[5]==0)
            and1();
        else if(IR.bit[15]==0 && IR.bit[14]==1 && IR.bit[13]==0 && IR.bit[12]==1 && IR.bit[5]==1)
            and2();
        else if(IR.bit[15]==0 && IR.bit[14]==0 && IR.bit[13]==0 && IR.bit[12]==0)
            br();
        else if(IR.bit[15]==1 && IR.bit[14]==1 && IR.bit[13]==0 && IR.bit[12]==0 && (IR.bit[5]==0 || IR.bit[6]==0 || IR.bit[7]==0))
            jmp();
        else if(IR.bit[15]==0 && IR.bit[14]==1 && IR.bit[13]==0 && IR.bit[12]==0 && IR.bit[11]==1)
            jsr();
        else if(IR.bit[15]==0 && IR.bit[14]==1 && IR.bit[13]==0 && IR.bit[12]==0 && IR.bit[11]==0)
            jsrr();
        else if(IR.bit[15]==1 && IR.bit[14]==1 && IR.bit[13]==0 && IR.bit[12]==0)
            ret();
        else if(IR.bit[15]==0 && IR.bit[14]==0 && IR.bit[13]==1 && IR.bit[12]==0)
            ld();
        else if(IR.bit[15]==1 && IR.bit[14]==0 && IR.bit[13]==1 && IR.bit[12]==0)
            ldi();
        else if(IR.bit[15]==0 && IR.bit[14]==1 && IR.bit[13]==1 && IR.bit[12]==0)
            ldr();
        else if(IR.bit[15]==1 && IR.bit[14]==1 && IR.bit[13]==1 && IR.bit[12]==0)
            lea();
        else if(IR.bit[15]==1 && IR.bit[14]==0 && IR.bit[13]==0 && IR.bit[12]==1)
            NoT();
        else if(IR.bit[15]==0 && IR.bit[14]==0 && IR.bit[13]==1 && IR.bit[12]==1)
            st();
        else if(IR.bit[15]==1 && IR.bit[14]==0 && IR.bit[13]==1 && IR.bit[12]==1)
            sti();
        else if(IR.bit[15]==0 && IR.bit[14]==1 && IR.bit[13]==1 && IR.bit[12]==1)
            str();
    }
private:
    void Run()
    {
        while(true)
        {
            fetch();
            decode();
        }
    }
    
    void LoadToMemory()
    {
        FILE *f;
        char c;
        f=fopen("Path to file","r");
        for (int i=0;i<Memory;i++)
        {
            for (int j=0;j<16;j++)
            {
                fscanf(f,"%c",&c);
                M[i].bit[j]=c-48;
            }
            fscanf(f,"%c",&c);
        }
        fclose(f);
    }
public:
    Computer()
    {
        LoadToMemory();
        Run();
    }
};

int main(int argc, const char * argv[]) {
    
    Computer LC3;
    return 0;
}
