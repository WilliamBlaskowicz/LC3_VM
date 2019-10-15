
#include <iostream>
#include <cstdio>
using namespace std;

#define Registers 8
#define Memory 65536

class Computer
{
    struct memory_unit
    {
        bool bit[16];
    };
    
    memory_unit R[Registers],M[Memory],PC;
    bool NPZ[3];
    
    class instruction
    {
        memory_unit cur;
        
        instruction(bool *A)
        {
            for(int i=0;i<16;i++)
                cur.bit[i]=A[i];
        }
        
        
        // Auxiliary functions
        
        void PC_incr(memory_unit *PC)
        {
            bool c=1;
            for (int i=0;i<16;i++)
            {
                PC[0].bit[i]=PC[0].bit[i]*(!c)+c*(!PC[0].bit[i]);
                c=PC[0].bit[i]*c;
            }
        }
        
        memory_unit SEXT11()
        {
            memory_unit rslt;
            for (int i=11;i<16;i++)
                rslt.bit[i]=cur.bit[10];
            for (int i=0;i<11;i++)
                rslt.bit[i]=cur.bit[i];
            return rslt;
        }
        
        
        memory_unit SEXT9()
        {
            memory_unit rslt;
            for (int i=9;i<16;i++)
                rslt.bit[i]=cur.bit[8];
            for (int i=0;i<9;i++)
                rslt.bit[i]=cur.bit[i];
            return rslt;
            
        }
        
        memory_unit SEXT5()
        {
            memory_unit rslt;
            for (int i=5;i<16;i++)
                rslt.bit[i]=cur.bit[4];
            for (int i=0;i<5;i++)
                rslt.bit[i]=cur.bit[i];
            return rslt;
        }
        
        memory_unit SEXT6()
        {
            memory_unit rslt;
            for (int i=6;i<16;i++)
                rslt.bit[i]=cur.bit[5];
            for (int i=0;i<6;i++)
                rslt.bit[i]=cur.bit[i];
            return rslt;
        }
        
        
        int bin2int(const memory_unit &a)
        {
            int rslt=0,c=1;
            for (int i=0;i<16;i++)
            {
                rslt+=a.bit[i]*c;
                c=c*2;
            }
            return rslt;
        }
        
        
        //implement npz up to and2
        
        void add1(memory_unit *R,bool *S)
        {
            int r1,r2;
            
            r1=cur.bit[6]+cur.bit[7]*2+cur.bit[8]*4;
            r2=cur.bit[0]+cur.bit[1]*2+cur.bit[2]*4;
            
            bool c=0;
            bool g;
            for(int i=0;i<16;i++)
            {
                g=R[r1].bit[i]*R[r2].bit[i]*c+R[r1].bit[i]*(!c)*(!R[r2].bit[i])+c*(!R[r1].bit[i])*(!R[r2].bit[i])+R[r2].bit[i]*(!c)*(!R[r1].bit[i]);
                R[cur.bit[9]+cur.bit[10]*2+cur.bit[11]*4].bit[i]=g;
                c=R[r1].bit[i]*R[r2].bit[i]+R[r1].bit[i]*c+c*R[r2].bit[i];
            }
            // NPZ initialisation
            if(R[cur.bit[9]+2*cur.bit[10]+4*cur.bit[11]].bit[15]==1)
            {
                S[0]=1;
                S[1]=0;
                S[2]=0;
            }
            else
            {
                for (int i=0;i<15;i++)
                    if(R[cur.bit[9]+2*cur.bit[10]+4*cur.bit[11]].bit[i]==1)
                    {
                        S[0]=0;
                        S[1]=1;
                        S[2]=0;
                        return;
                    }
                S[0]=0;
                S[1]=0;
                S[2]=1;
            }
        }
        
        void add2(memory_unit *R,bool *S)
        {
            int dr=cur.bit[9]+cur.bit[10]*2+cur.bit[11]*4;
            int sr=cur.bit[6]+cur.bit[7]*2+cur.bit[8]*4;
            bool c=0;
            memory_unit t=SEXT5();
            for(int i=0;i<5;i++)
            {
                R[dr].bit[i]=R[sr].bit[i]*c*t.bit[i]+R[sr].bit[i]*(!c)*(!t.bit[i])+c*(!R[sr].bit[i])*(!t.bit[i])+t.bit[i]*(!c)*(!R[sr].bit[i]);
                c=R[sr].bit[i]*c+R[sr].bit[i]*t.bit[i]+t.bit[i]*c;
            }
            if (R[dr].bit[15]==1)
            {
                S[0]=1;
                S[1]=0;
                S[2]=0;
            }
            else
            {
                for (int i=0;i<15;i++)
                    if(R[dr].bit[i]==1)
                    {
                        S[0]=0;
                        S[1]=1;
                        S[2]=0;
                        return;
                    }
                S[0]=0;
                S[1]=0;
                S[2]=1;
            }
        }
        
        void and1(memory_unit *R,bool *S)
        {
            int dr=cur.bit[9]+cur.bit[10]*2+cur.bit[11]*4;
            int r1,r2;
            r1=cur.bit[0]+cur.bit[1]*2+cur.bit[2]*4;
            r2=cur.bit[6]+cur.bit[7]*2+cur.bit[8]*4;
            
            for(int i=0;i<16;i++)
                R[dr].bit[i]=R[r1].bit[i]*R[r2].bit[i];
            
            if (R[dr].bit[15]==1)
            {
                S[0]=1;
                S[1]=0;
                S[2]=0;
            }
            else
            {
                for (int i=0;i<15;i++)
                    if(R[dr].bit[i]==1)
                    {
                        S[0]=0;
                        S[1]=1;
                        S[2]=0;
                        return;
                    }
                S[0]=0;
                S[1]=0;
                S[2]=1;
            }
        }
        
        void and2(memory_unit *R,bool *S)
        {
            int dr=cur.bit[9]+cur.bit[10]*2+cur.bit[11]*4;
            memory_unit temp=SEXT5();
            for(int i=0;i<16;i++)
                R[dr].bit[i]=R[dr].bit[i]*temp.bit[i];
            
            if (R[dr].bit[15]==1)
            {
                S[0]=1;
                S[1]=0;
                S[2]=0;
            }
            else
            {
                for (int i=0;i<15;i++)
                    if(R[dr].bit[i]==1)
                    {
                        S[0]=0;
                        S[1]=1;
                        S[2]=0;
                        return;
                    }
                S[0]=0;
                S[1]=0;
                S[2]=1;
            }
        }
        
        //   BEWARE BUGS
        void br(bool *S)
        {
            ;
        }
        
        void jmp(memory_unit *R,memory_unit *PC)
        {
            PC[0]=R[cur.bit[6]+2*cur.bit[7]+4*cur.bit[8]];
        }
        
        void jsr(memory_unit *PC,memory_unit *R)
        {
            memory_unit sext=SEXT11();
            bool c=0;
            R[7]=PC[0];
            for (int i=0;i<16;i++)
            {
                PC[0].bit[i]=PC[0].bit[i]*sext.bit[i]*c+PC[0].bit[i]*(!c)*(!sext.bit[i])+c*(!PC[0].bit[i])*(!sext.bit[i])+sext.bit[i]*(!c)*(!PC[0].bit[i]);
                c=PC[0].bit[i]*c+PC[0].bit[i]*sext.bit[i]+sext.bit[i]*c;
            }
        }
        
        void jsrr(memory_unit *R,memory_unit *PC)
        {
            R[7]=PC[0];
            PC[0]=R[cur.bit[6]+2*cur.bit[7]+4*cur.bit[8]];
        }
        
        
        void ld(memory_unit *R,memory_unit *M,memory_unit *PC)
        {
            int dr=cur.bit[9]+cur.bit[10]*2+cur.bit[11]*4;
            memory_unit d,s=SEXT11();
            bool c=0;
            for (int i=0;i<16;i++)
            {
                d.bit[i]=PC[0].bit[i]*s.bit[i]*c+PC[0].bit[i]*(!c)*(!s.bit[i])+c*(!PC[0].bit[i])*(!s.bit[i])+s.bit[i]*(!PC[0].bit[i])*(!c);
                c=PC[0].bit[i]*c+s.bit[i]*c+PC[0].bit[i]*s.bit[i];
            }
            R[dr]=M[bin2int(d)];
        }
        
        void ldi(memory_unit *R,memory_unit *PC,memory_unit *M)
        {
            int dr=cur.bit[9]+2*cur.bit[10]+4*cur.bit[11];
            memory_unit d,s=SEXT11();
            bool c=0;
            for (int i=0;i<16;i++)
            {
                d.bit[i]=PC[0].bit[i]*s.bit[i]*c+PC[0].bit[i]*(!c)*(!s.bit[i])+c*(!PC[0].bit[i])*(!s.bit[i])+s.bit[i]*(!PC[0].bit[i])*(!c);
                c=PC[0].bit[i]*c+s.bit[i]*c+PC[0].bit[i]*s.bit[i];
            }
            R[dr]=M[bin2int(M[bin2int(d)])];
        }
        
        void ldr(memory_unit *R,memory_unit *M)
        {
            memory_unit d,of=SEXT6();
            int dr=cur.bit[9]+2*cur.bit[10]+4*cur.bit[11],sr=cur.bit[6]+2*cur.bit[7]+4*cur.bit[8];
            bool c=0;
            for (int i=0;i<16;i++)
            {
                d.bit[i]=R[sr].bit[i]*c*of.bit[i]+R[sr].bit[i]*(!c)*(!of.bit[i])+(!R[sr].bit[i])*c*(!of.bit[i])+of.bit[i]*(!c)*(!R[sr].bit[i]);
                c=R[sr].bit[i]*c+R[sr].bit[i]*of.bit[i]+of.bit[i]*c;
            }
            R[dr]=M[bin2int(d)];
        }
        
        void lea(memory_unit *R,memory_unit *PC)
        {
            int dr=cur.bit[9]+2*cur.bit[10]+4*cur.bit[11];
            memory_unit of=SEXT9();
            bool c=0;
            for (int i=0;i<16;i++)
            {
                R[dr].bit[i]=PC[0].bit[i]*of.bit[i]*c+PC[0].bit[i]*(!c)*(!of.bit[i])+c*(!PC[0].bit[i])*(!of.bit[i])+of.bit[i]*(!PC[0].bit[i])*(!c);
                c=PC[0].bit[i]*c+of.bit[i]*c+PC[0].bit[i]*of.bit[i];
            }
        }
        
        void NoT(memory_unit *R,bool *S)
        {
            int dr=cur.bit[9]+2*cur.bit[10]+4*cur.bit[11],sr=cur.bit[6]+2*cur.bit[7]+4*cur.bit[8];
            for (int i=0;i<16;i++)
                R[dr].bit[i]=!R[sr].bit[i];
            
            if (R[dr].bit[15]==1)
            {
                S[0]=1;
                S[1]=0;
                S[2]=0;
            }
            else
            {
                for (int i=0;i<15;i++)
                    if(R[dr].bit[i]==1)
                    {
                        S[0]=0;
                        S[1]=1;
                        S[2]=0;
                        return;
                    }
                S[0]=0;
                S[1]=0;
                S[2]=1;
            }
            
        }
        
        void ret(memory_unit *R,memory_unit *PC)
        {
            PC[0]=R[7];
        }
        
        void st(memory_unit *R,memory_unit *M,memory_unit *PC)
        {
            int sr=cur.bit[9]+2*cur.bit[10]+4*cur.bit[11];
            memory_unit d,of=SEXT9();
            bool c=0;
            for (int i=0;i<16;i++)
            {
                d.bit[i]=of.bit[i]*c*PC[0].bit[i]+of.bit[i]*(!c)*(!PC[0].bit[i])+c*(!PC[0].bit[i])*(!of.bit[i])+PC[0].bit[i]*(!c)*(!of.bit[i]);
                c=of.bit[i]*c+PC[0].bit[i]*c+of.bit[i]*PC[0].bit[i];
            }
            M[bin2int(d)]=R[sr];
        }
        
        void sti(memory_unit *R,memory_unit *M,memory_unit *PC)
        {
            memory_unit d,of=SEXT9();
            int sr=cur.bit[9]+2*cur.bit[10]+4*cur.bit[11];
            bool c=0;
            
            for (int i=0;i<16;i++)
            {
                d.bit[i]=PC[0].bit[i]*of.bit[i]*c+PC[0].bit[i]*(!c)*(!of.bit[i])+c*(!PC[0].bit[i])*(!of.bit[i])+of.bit[i]*(!c)*(!PC[0].bit[i]);
                c=PC[0].bit[i]*c+PC[0].bit[i]*of.bit[i]+of.bit[i]*c;
            }
            M[bin2int(M[bin2int(d)])]=R[sr];
        }
        
    
        // S to be replaced at runtime with NPZ
        void execute(memory_unit *R,bool *S,memory_unit *PC,memory_unit *M)
        {
            PC_incr(PC);
            
            if(cur.bit[15]==0 && cur.bit[14]==0 && cur.bit[13]==0 && cur.bit[12]==1 && cur.bit[5]==0) //ADD1
                add1(R,S);
            else if(cur.bit[15]==0 && cur.bit[14]==0 && cur.bit[13]==0 && cur.bit[12]==1 && cur.bit[5]==1) //ADD2
                add2(R,S);
            else if(cur.bit[15]==0 && cur.bit[14]==1 && cur.bit[13]==0 && cur.bit[12]==1 && cur.bit[5]==0) //AND1
                and1(R,S);
            else if(cur.bit[15]==0 && cur.bit[14]==1 && cur.bit[13]==0 && cur.bit[12]==1 && cur.bit[5]==1) //AND2
                and2(R,S);
            else if(cur.bit[15]==0 && cur.bit[14]==0 && cur.bit[13]==0 && cur.bit[12]==0) //BR
                br(S);
            else if(cur.bit[15]==1 && cur.bit[14]==1 && cur.bit[13]==0 && cur.bit[12]==0) //JMP
                jmp(R,PC);
            else if(cur.bit[15]==0 && cur.bit[14]==1 && cur.bit[13]==0 && cur.bit[12]==0 && cur.bit[11]==1) //JSR
                jsr(PC,R);
            else if(cur.bit[15]==0 && cur.bit[14]==1 && cur.bit[13]==0 && cur.bit[12]==0 && cur.bit[11]==0) //JSRR
                jsrr(R,PC);
            else if(cur.bit[15]==0 && cur.bit[14]==0 && cur.bit[13]==1 && cur.bit[12]==0) // LD
                ld(R,M,PC);
            else if(cur.bit[15]==1 && cur.bit[14]==0 && cur.bit[13]==1 && cur.bit[12]==0) //LDI
                ldi(R,PC,M);
            else if(cur.bit[15]==0 && cur.bit[14]==1 && cur.bit[13]==1 && cur.bit[12]==0) //LDR
                ldr(R,M);
            else if(cur.bit[15]==1 && cur.bit[14]==1 && cur.bit[13]==1 && cur.bit[12]==0) //LEA
                lea(R,PC);
            else if(cur.bit[15]==1 && cur.bit[14]==0 && cur.bit[13]==0 && cur.bit[12]==1) //NOT
                NoT(R,S);
            else if(cur.bit[15]==1 && cur.bit[14]==1 && cur.bit[13]==0 && cur.bit[12]==0 && cur.bit[6]==1 && cur.bit[7]==1 && cur.bit[8]==1) //RET
                ret(R,PC);
            else if(cur.bit[15]==0 && cur.bit[14]==0 && cur.bit[13]==1 && cur.bit[12]==1) //ST
                st(R,M,PC);
            else if(cur.bit[15]==1 && cur.bit[14]==0 && cur.bit[13]==1 && cur.bit[12]==1) //STI
                sti(R,M,PC);
        }
    };
    
public:
    void LoadToMemory()
    {
        FILE *f;
        char c;
        f=fopen("/Users/andreaskosmaskakolyris/Desktop/ex1.lc3","r");
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
};

int main(int argc, const char * argv[]) {
    
    Computer LC3;
    LC3.LoadToMemory();
    return 0;
}
