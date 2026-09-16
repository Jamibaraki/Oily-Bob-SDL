#pragma once

class Hiscores{

    public: int hiscores[10];


    public: int setScore(int score){
        for(int i=9;i>=0;i--){
            if(score > hiscores[i]){
                if(i<9){
                    hiscores[i+1]=hiscores[i];
                }
                hiscores[i]=score;
            }
        }
    }
};
