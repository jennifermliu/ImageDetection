int SB=14000;
int SM=2000;
int LVM=-10;
int UVM=10;
int LVB=-7000;
int UVB=7000;

float fromItoV(int I, int S, int LV, int UV)
{
    float Itemp=(float)I;
    float UVtemp=(float)UV;
    float LVtemp=(float)LV;
    float Stemp=(float)S;
    float Vrange=UVtemp-LVtemp;
    float V=Itemp*Vrange/Stemp+LVtemp;
//    printf("V is: %f ", V);
    return V;
}

float fromVtoI(float V, int S, int LV, int UV)
{
    float UVtemp=(float)UV;
//    printf("UV is %d ", UV);
//    printf("UVtemp is %f ", UVtemp);
    float LVtemp=(float)LV;
    float Stemp=(float)S;
    float Vrange=UVtemp-LVtemp;
    float I=(V-LVtemp)*Stemp/Vrange;
    return I;
}

void VoteforYX(int **VS, int y, int x)
{
    for(int IM=0; IM<SM+1; IM++)
    {
        float m=fromItoV(IM,SM,LVM,UVM);
        float b=479-y-x*m;
        float I=fromVtoI(b,SB,LVB,UVB);
        int IB=(int)I;
        VS[IB][IM]++;
    }
}

void VoteLine(int **binary,int **VS)//vote for each m and b at each pixel
{
    for(int y=0;y<HEIGHT; y++)
    {
        for(int x=0;x<WIDTH; x++)
        {
            if(binary[y][x]==1)//vote only if this pixel is shape
            {
                VoteforYX(VS,y,x);
            }
//            printf("x and y are %d and %d ", x,y);
        }
    }
}
void ConvertToBinary(unsigned char ***image, int **binary)//determine if each pixel is background or shape
{
    for(int y=0; y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {
            if((unsigned int)image[0][y][x]==255 && (unsigned int)image[1][y][x]==255 &&(unsigned int)image[2][y][x]==255)//white if all 3 are 0
            {
                binary[y][x]=0;//0 means background
            }
            else
            {
                binary[y][x]=1;//1 means anything other than white
            }
        }
    }
}

void detect_line (unsigned char ***image, float *m, float *b)
{
	// PUT YOUR CODE HERE
	// access the image using "image[chanel][y][x]"
	// set m and b using the * operator
	// to create a bidimentional structure you can use "int **space=init_space(heigh, width);"
    
    // create a 2d array to record whether each pixel is shape or background
    int **binary=init_space(HEIGHT,WIDTH);
    
    for(int y=0; y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {
            binary[y][x]=0;
        }
    }
    
    ConvertToBinary(image,binary);
    
//    for(int y=0; y<HEIGHT;y++)
//    {
//        for(int x=0; x<WIDTH; x++)
//        {
//            if(binary[y][x]!=0)
//            {
//                printf("binary[%d][%d] is: %d ", y,x,binary[y][x]);
//
//            }
//        }
//    }
    
    // create a 2d array to record whether vote for each m and b combination
    int **VS=init_space(SB+1,SM+1);
    
    for(int y=0; y<SB+1; y++)
    {
        for(int x=0; x<SM+1; x++)
        {
            VS[y][x]=0;
        }
    }
    
    VoteLine(binary,VS);
    
    // determine the m and b combinations that get the most vote
    int mI=0;
    int bI=0;
    int best=VS[0][0];
    
    // update best when current is better than best
    for(int litb=0; litb<SB+1;litb++)
    {
        for(int litm=0; litm<SM+1;litm++)
        {
            if(VS[litb][litm]>best)
            {
                best=VS[litb][litm];
                mI=litm;
                bI=litb;
            }
        }
    }
    
//    printf("mI is %d and bI is %d \n",mI,bI);
  
    *m=fromItoV(mI,SM,LVM,UVM);
    *b=fromItoV(bI,SB,LVB,UVB);

}

int SB2=14000;
int SM2=2000;
int LVM2=-1;
int UVM2=1;
int LVB2=-7000;
int UVB2=7000;
int T=130;
float TB=50;
float TM=0.1;
int length=100;

float abs(float value)
{
    if(value<0)
    {
        return -value;
    }
    else
    {
        return value;
    }
}

int FindEdge(int **binary,int **edges)
{
    for(int y=1; y<HEIGHT-1; y++)
    {
        for(int x=1; x<WIDTH-1;x++)
        {
            if(binary[y][x]==1 && (binary[y-1][x-1]==0 or binary[y-1][x]==0 or binary[y-1][x+1]==0 or binary[y][x-1]==0 or binary[y][x]==0 or binary[y][x+1]==0 or binary[y+1][x-1]==0 or binary[y+1][x]==0 or binary[y+1][x+1]==0))//if current pixel is part of a shape but one of the surrounding pixel is not
            {
                edges[y][x]=1;//set it to 1 if it's part of an edge
            }
        }
    }
}

void VoteforYX2(int **VS, int y, int x)
{
    for(int IM=0; IM<SM+1; IM++)
    {
        float m=fromItoV(IM,SM,LVM,UVM);
        float b=479-y-x*m;
        float I=fromVtoI(b,SB,LVB,UVB);
        int IB=(int)I;
        VS[IB][IM]++;
    }
}
void VoteforXY2(int **VS, int x, int y)
{
    for(int IM=0; IM<SM+1; IM++)
    {
        float m=fromItoV(IM,SM,LVM,UVM);
        float b=479-x-y*m;
        float I=fromVtoI(b,SB,LVB,UVB);
        int IB=(int)I;
        VS[IB][IM]++;
    }
}

void VoteLine_p(int **edges,int **VSX, int **VSY)
{
    for(int y=0;y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {
            if(edges[y][x]==1)
            {
//                printf("edge at edges[%d][%d] ", y,x);
                VoteforYX2(VSX,y,x);
                VoteforXY2(VSY,x,y);
            }
        }
    }
}

bool proximity(float m, float b, float *rm, float *rb)
{
    for(int i=0; i<length; i++)
    {
        if(abs(m-rm[i])<TM && abs(b-rb[i])<TB)
        {
            return false;
        }
    }
    return true;
}
int filter(float *ms, float *bs)
{
//    float lowm[length];
//    for(int i=0; i<length;i++)
//    {
//        lowm[i]=0;
//    }
//    float lowb[length];
//    for(int i=0; i<length;i++)
//    {
//        lowb[i]=0;
//    }
//
//    float abslowm=10;
//    float abslowb=1000;
//    int bindex=0;
//    int numlines=0;
//    for(int i=0;i<length;i++)
//    {
//        if(ms[i]!=0 or bs[i]!=0)
//        {
//            numlines++;
//            if(abs(bs[i]<abslowb))
//            {
//                abslowb=abs(bs[i]);
//                bindex=i;
//            }
//        }
//    }
    
    float rm[length];
    for(int i=0; i<length;i++)
    {
        rm[i]=0;
    }
    float rb[length];
    for(int i=0; i<length;i++)
    {
        rb[i]=0;
    }
    int count=0;
//
//    rm[0]=ms[bindex];
//    rb[0]=bs[bindex];
//    printf("rm[0] is %f \n",rm[0]);
//    printf("rb[0] is %f \n",rb[0]);
    
    
//    int rindex=1;
//    if(rb[0]!=0 or rm[0]!=0)
//    {
//        temp++;
//    }
//    printf("temp is %d \n",temp);
//    for(int i=0; i<length;i++)
//    {
//        if(proximity(ms[i],bs[i],rm,rb))
//        {
//            rm[rindex]=ms[i];
//            rb[rindex]=bs[i];
//            temp++;
//            rindex++;
//        }
//        
//    }
//    for(int i=0; i<length;i++)
//    {
//        if(abs(bs[i])<TB)
//        {
//            rm[i]=ms[i];
//            rb[i]=bs[i];
//        }
//    }
//    
    int btotalzeros=0;
    int mtotalzeros=0;

    
    for(int i=0; i<length;i++)
    {
        for(int j=0; j<length;j++)
        {
            if(rb[j]==0)
            {
                btotalzeros++;
            }
            if(rm[j]==0)
            {
                mtotalzeros++;
            }
        }
        if(abs(bs[i])<TB &&abs(ms[i])<TM)
        {
            if(ms[i]==0 or bs[i]==0)
            {
                if(btotalzeros==length && mtotalzeros==length)
                {
                    rm[i]=ms[i];
                    rb[i]=bs[i];
                    count++;
                }
            }
            
        }
        else if(proximity(ms[i],bs[i],rm,rb))
        {
            rm[i]=ms[i];
            rb[i]=bs[i];
            count++;
           
        }
        
    }
    return count;
}


void findLines(int **VS, float *ms, float *bs)
{
    int temp=0;
    for(int bI=0; bI<SB2+1; bI++)
    {
        for(int mI=0; mI<SM2+1; mI++)
        {
            if(VS[bI][mI]>T)
            {
//                printf("VS[%d][%d] is: %d ",bI,mI,VS[bI][mI]);
                float b=fromItoV(bI,SB2,LVB2,UVB2);
                float m=fromItoV(mI,SM2,LVM2,UVM2);
                bs[temp]=b;
                ms[temp]=m;
                temp++;
            }
        }
    }
   
}

int detect_polygon (unsigned char ***image)
{
	// PUT YOUR CODE HERE
	// access the image using "image[chanel][y][x]"
	// return the number of sides of the polygon
	// to create a bidimentional structure you can use "int **space=init_space(heigh, width);"
    
    int **binary=init_space(HEIGHT,WIDTH);
    
    for(int y=0; y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {
            binary[y][x]=0;
        }
    }
    
    ConvertToBinary(image,binary);
    
    int **edges=init_space(HEIGHT,WIDTH);
    
    for(int y=0; y<HEIGHT; y++)
    {
        for(int x=0; x<WIDTH; x++)
        {
            edges[y][x]=0;
        }
    }
    
    FindEdge(binary,edges);
    
    int **VSX=init_space(SB+1,SM+1);
    
    for(int y=0; y<SB2+1; y++)
    {
        for(int x=0; x<SM2+1; x++)
        {
            VSX[y][x]=0;
        }
    }
    
    
    int **VSY=init_space(SB2+1,SM2+1);
    
    for(int y=0; y<SB2+1; y++)
    {
        for(int x=0; x<SM2+1; x++)
        {
            VSY[y][x]=0;
        }
    }
    
    VoteLine_p(edges,VSX,VSY);
//    for(int y=0; y<SB2+1; y++)
//    {
//        for(int x=0; x<SM2+1; x++)
//        {
//            printf("VSX[y][x] is %d ", VSX[y][x]);
//        }
//    }
    
    float msx[length];
    for(int i=0; i<length;i++)
    {
        msx[i]=0;
    }
    float msy[length];
    for(int i=0; i<length;i++)
    {
        msy[i]=0;
    }
    float bsx[length];
    for(int i=0; i<length;i++)
    {
        bsx[i]=0;
    }
    float bsy[length];
    for(int i=0; i<length;i++)
    {
        bsy[i]=0;
    }

    findLines(VSX, msx, bsx);
    findLines(VSY, msy, bsy);
//    for(int i=0; i<length;i++)
//    {
//        if(msx[i]!=0 or bsx[i]!=0)
//        {
//            printf("there is a X line at m=%f and b=%f \n",msx[i],bsx[i]);
//        }
//    }
//    for(int i=0; i<length;i++)
//    {
//        if(msy[i]!=0 or bsy[i]!=0)
//        {
//            printf("there is a Y line at m=%f and b=%f \n",msy[i],bsy[i]);
//        }
//    }
    
    int Xcount=filter(msx, bsx);
    int Ycount=filter(msy, bsy);

    
    int sides=Xcount+Ycount;
//    printf("filter(msx,bsx) is %d ",filter(msx, bsx));
//    printf("filter(msy,bsy) is %d ",filter(msy, bsy));
    
	return sides;
}

int circle_on_top (unsigned char ***image)
{
	// PUT YOUR CODE HERE
	// access the image using "image[chanel][y][x]"
	// return RED or BLUE
	// to create a bidimentional structure you can use "int **space=init_space(heigh, width);"
	return RED;
}
