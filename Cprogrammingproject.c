# include<stdio.h>
    #include<math.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include<time.h>
    #include <ctype.h>
    #include <unistd.h> // for usleep function
    #define MAX_SCENES 100
    #define NUM_QUESTIONS 5
    // Define color escape sequences
    #define RESET   "\033[0m"
    #define BLACK   "\033[0;30m"
    #define RED     "\033[0;31m"
    #define GREEN   "\033[0;32m"
    #define YELLOW  "\033[0;33m"
    #define BLUE    "\033[0;34m"
    #define MAGENTA "\033[0;35m"
    #define CYAN    "\033[0;36m"
    #define WHITE   "\033[0;37m"
    typedef struct stats
        {
            int hit_point,attack,defence,mana,speed,current_hit_point,current_attack,current_defence,current_mana,current_speed,damage,attribute[9],level ,experience,mana_resistance,current_mana_resistance;
        } st;
    bool won;

    // Function to set text color to black
    void setBlackColor() {
        printf(BLACK);
    }

    // Function to set text color to red
    void setRedColor() {
        printf(RED);
    }

    // Function to set text color to green
    void setGreenColor() {
        printf(GREEN);
    }

    // Function to set text color to yellow
    void setYellowColor() {
        printf(YELLOW);
    }

    // Function to set text color to blue
    void setBlueColor() {
        printf(BLUE);
    }

    // Function to set text color to magenta
    void setMagentaColor() {
        printf(MAGENTA);
    }

    // Function to set text color to cyan
    void setCyanColor() {
        printf(CYAN);
    }

    // Function to set text color to white
    void setWhiteColor() {
        printf(WHITE);
    }

    // Function to reset text color to default
    void resetColor() {
        printf(RESET);
    }

    void dis_hit_point(int hit_point1,int hit_point2,char *name,int h1,int h2);     //function to display hit points
    void dis_stats(st *p,char *name);                                               //function to display stats
    void dis_attribute(st *p);                                                      //function to display attributes
    void dis_att(int q[],int power[],st *player);                                   //function to display moves
    int damage(int hit_point,int a,int d,int p);                                    //function to calculate damage
    void attack(int quest[],st *boss,st *player,int power[],float tc[][9]);         //function for attack
    void attack_boss(st *boss,st *player,int power[]);                              //function for boss attack
    void dis_item(int item[],st *player,st *boss);                                  //function to display items
    void change(st *player,st *boss,int ch,int cat);                                //function for non damage moves
    void update_stats(st* boss,int hp,int att,int def,int ma, int mares,int spd,int lv,int attrib,int exp);         //function to update stats
    int battle(st *player,st *boss, char *name,int power[],int quest[],float type_chart[][9],int item[]);           //function for battle
    int update_power(int pow,int lv);                                               //function to update power
    void update_level(st *player,st*boss,int attrib);                               //function to update level
    void level_up(st *player,int level_increase,int attrib);                        //function to level up

    void battle_mechanincs()      
    {
        char name[20];
        strcpy(name, "you");
        int item[40],quest[40],ch=1;
        float type_chart[9][9]={1,0.5,1,2,1,2,1,1,2, 2,1,0.5,1,1,0.5,1,1,1, 1,2,1,0.5,2,1,1,1,1, 1,1,2,1,0.5,1,1,1,0.5, 1,1,0.5,2,1,1,1,1,2,  0.5,2,2,2,1,0.5,1,1,0.5, 0.5,1,1,1,0.5,1,0.5,0.5,1, 1,1,1,1,1,1,2,0.5,1, 0.5,1,1,1,1,1,1,1,0.5};
        //creating type chart for effectiveness of a move
        int power[27]={-10,0,0,-10,0,10,0,10,0,-15,15,0,-15,15,0,-15,15,0,20,-20,0,-25,0,30,15,0,15};
        
        for(int i=0;i<27;i++)
        {   
            quest[i]=1;//quest is for no. of moves unlocked
            item[i]=10;// for items unlocked
        }

        st *player,*boss;//pointers for player and opponent
        player=(st *)malloc(sizeof(st));
        boss=(st *)malloc(sizeof(st));

        printf("Enter name:\n");
        scanf("%[^\n]",name);
        printf("\e[1;1H\e[2J");//for clear screen

        update_stats(player,25,50,50,50,50,50,10,100000000,0);//assigning stats to player
        update_stats(boss,25,25,25,25,25,25,10,10000000,1000);//creating enemy
        
        if(battle(player,boss,name,power,quest,type_chart, item))
        {
            update_level(player,boss,100000000);
            dis_stats(player,name);
            won=true;
        }
    }
    //for battle
    int battle(st *player,st *boss, char *name,int power[],int quest[],float type_chart[][9],int item[])
    {
    int choice;
    printf("\e[1;1H\e[2J");//for clear screen 
    while(((*player).current_hit_point>0)&&((*boss).current_hit_point>0))
    {
        Battle_start://label for goto statement
        dis_hit_point((*player).current_hit_point,(*boss).current_hit_point,name,(*player).hit_point,(*boss).hit_point);
        printf("1.ATTACK    2.ITEMS\n3.STATS     4.RUN\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1://for attack
                printf("\e[1;1H\e[2J");
                dis_hit_point((*player).current_hit_point,(*boss).current_hit_point,name,(*player).hit_point,(*boss).hit_point);
                (*player).damage=(*boss).damage=0;
                attack(quest, boss,player, power,type_chart);
                attack_boss( boss,player, power);
                if((*player).current_speed>(*boss).current_speed)//damage priority based upon speed
                {
                    (*boss).current_hit_point-=(*player).damage;
                    if((*boss).current_hit_point==0)
                        break;
                        (*player).current_hit_point-=(*boss).damage;
                    }
                    else
                    {
                    (*player).current_hit_point-=(*boss).damage;
                    
                    if((*player).current_hit_point==0)
                        break;
                        (*boss).current_hit_point-=(*player).damage;
                }
                break;
                case 2: //for items
                    printf("\e[1;1H\e[2J");
                    dis_item(item,player, boss);
                attack_boss( boss,player, power);
                (*player).current_hit_point-=(*boss).damage;
                    break;  
                case 3: //for stats
                    printf("\e[1;1H\e[2J");
                    dis_stats(player,name);
                    dis_stats(boss,"Boss");
                    printf("\nEnter any key: ");
                    scanf("%d",&choice);
                break;
                case 4://for forfeit
                    (*player).current_hit_point=0;
                    break;    
                    default:
                    printf("\e[1;1H\e[2J");
                    goto Battle_start;
                break;
        }
            printf("\e[1;1H\e[2J");
    }
    if((*player).current_hit_point>0&&(*boss).current_hit_point==0)
    {
        printf("You have won");
        return 1;
    }
    else
    {
        printf("You lost");
        return 0;
    }
    }
    void dis_hit_point(int hit_point1,int hit_point2,char *name,int h1,int h2)//displaying hp
    {
        int n;
        printf(" YOU");
        for(int i=1;i<h1;i++)
            printf(" ");//for space between name and boss name
        printf("Boss \n");
        printf("HP:");//player hp
        for(int i=1;i<=hit_point1;i++)
            printf("*");
        for(int i=1;i<=h1-hit_point1;i++)
            printf("-");
        printf("  HP: ");//boss hp
        for(int i=1;i<=hit_point2;i++)
        printf("*");
        for(int i=1;i<=h2-hit_point2;i++)
            printf("-");
        printf("\n");
    }
    //assigning stats
    void update_stats(st* boss,int hp,int att,int def,int ma, int mares,int spd,int lv,int attrib,int exp)
    {
        (*boss).hit_point=(*boss).current_hit_point=hp;
        (*boss).attack=(*boss).current_attack=att;
        (*boss).defence=(*boss).current_defence=def;
        (*boss).mana=(*boss).current_mana=ma;
        (*boss).speed=(*boss).current_speed=spd;
        (*boss).mana_resistance=(*boss).current_mana_resistance=mares;
        (*boss).level=lv;
        (*boss).experience=exp;
        for(int i=8;i>=0;i--)
        {
            (*boss).attribute[i]=attrib%10;
            attrib/=10;
        }
    }
    //displaying stats of the player
    void dis_stats(st *player,char *name)
    {
        int choice;
        printf("\n%s:\nHit point: %d/%d\nLevel:%d\nExperience points:%d/%d\nAttack: %d/%d\nDefence: %d/%d\nMana: %d/%d\nMana Resistance: %d/%d\nSpeed: %d/%d\n",name,(*player).current_hit_point,(*player).hit_point,(*player).level,(*player).experience,(*player).level*50,(*player).current_attack,(*player).attack,(*player).current_defence,(*player).defence,(*player).current_mana,(*player).mana,(*player).current_mana_resistance,(*player).mana_resistance,(*player).current_speed,(*player).speed);
        dis_attribute(player);//displaying attributes
    }
    //for displaying attributes
    void dis_attribute(st *player)
    {
        printf("Attributes: ");
        for(int i=0;i<9;i++)
        {
            switch (i)
            {
            case 0:
                if((*player).attribute[i]==1)
                    printf("Fire ");
                break;
            case 1:
                if((*player).attribute[i]==1)
                    printf("Water ");
                break;
            case 2:
                if((*player).attribute[i]==1)
                    printf("Ground ");
                break;
            case 3:
                if((*player).attribute[i]==1)
                    printf("Wind ");
                break;
            case 4:
                if((*player).attribute[i]==1)
                    printf("Lightening ");
                break;
            case 5:
                if((*player).attribute[i]==1)
                    printf("Ice ");
                break;
            case 6:
                if((*player).attribute[i]==1)
                    printf("Dark ");
                break;
            case 7:
                if((*player).attribute[i]==1)
                    printf("Light ");
                break;
            case 8:
                if((*player).attribute[i]==1)
                    printf("Metal ");
                break;
            default:
                break;
            }
        }
    }
    //for displaying moves
    void dis_att(int q[],int power[], st *player)
    {
        att:
        printf("| Move Name          | Power | Attribute | Type | Description \n");
        for(int i=0;i<27;i++)
        {
                    if(q[i]==1&&i==0)
                    printf("%d. Flame Burst        | %d    | Fire      | Mana | A burst of intense flames that engulfs the target.\n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==1)
                    printf("%d. Fire Dance         | %d    | Fire      | Mana | The user dances amidst flames, doubling its mana stat. \n",i+1,update_power(power[i],player->level)); 
            else if(q[i]==1&&i==2)
                    printf("%d. Fire Shield        | %d    | Fire      | Mana | Envelops the user in a protective shield of flames, doubles mana resistance.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==3)
                    printf("%d. Hydro Pump         | %d    | Water     | Mana | Releases a high-pressure stream of water at the opponent with immense force.\n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==4)
                    printf("%d. Aqua Ring          | %d    | Water     | Mana | Creates a swirling ring of water around the user, restores 25%% of health .\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==5)
                    printf("%d. Torrential Surge   | %d    | Water     | Phy  | Unleashes a massive wave that crashes onto the opponent.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==6)
                    printf("%d. Quicksand Trap     | %d    | Ground    | Phy  | Sets a trap of quicksand that ensnares the opponent, lowering their deffence by 25%% .\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==7)
                    printf("%d. Earthquake         | %d    | Ground    | Phy  | Causes a powerful tremor that shakes the ground.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==8)
                    printf("%d. Earth Barrier      | %d    | Ground    | Mana | Erects a barrier of earth around the user, doubles deffence.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==9)
                    printf("%d. Gale Force         | %d    | Wind      | Mana | Unleashes a powerful gust of wind.\n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==10)
                    printf("%d. Razor Wing         | %d    | Wind      | Phy  | Channels a cutting wind that slices through the opponent's defenses.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==11)
                    printf("%d. Cyclone Barrier    | %d    | Wind      | Mana | Erects a swirling cyclone barrier around the user that increases the user's speed.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==12)
                    printf("%d. Thunderbolt        | %d    | Lightning | Mana | Fires a bolt of lightning at the opponent.\n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==13)
                    printf("%d. Thundher Punch     | %d    | Lightning | Phy  | Unleashes a powerful punch of electricity that shocks the opponent.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==14)
                    printf("%d. Static Field       | %d    | Lightning | Mana | Creates a field of static electricity around the user, restore 25%% of mana.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==15)
                    printf("%d. Frostbite          | %d    | Ice       | Mana | Freezes the opponent with an icy chill. \n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==16)
                    printf("%d. Ice Shard          | %d    | Ice       | Phy  | Hurls a shard of ice at the opponent with incredible speed.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==17)
                    printf("%d. Ice Shield         | %d    | Ice       | Mana | Creates a shield of ice around the foe, reducing its mana resistance by 25%% .\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==18)
                    printf("%d. Shadow Strike      | %d    | Dark      | Phy  | Engulfs the opponent in darkness before striking with a powerful shadowy force. \n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==19)
                    printf("%d. Nightmare          | %d    | Dark      | Mana | Inflicts a haunting nightmare on the opponent. \n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==20)
                    printf("%d. Dark Veil          | %d    | Dark      | Mana | Shrouds the user in a veil of darkness,increases both attack and mana by 25%% .\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==21)
                    printf("%d. Holy Light         | %d    | Light     | Mana | Channels a radiant beam of holy light that purges darkness and deals damage to dark-aligned foes.\n",i+1,-update_power(power[i],player->level));
            else if(q[i]==1&&i==22)
                    printf("%d. Divine Blessing    | %d    | Light     | Mana | Bestows a divine blessing upon the user,restors 50%% of health.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==23)
                    printf("%d. Sword of Light     | %d    | Light     | Phy  | Radiates a sword of bright light that illuminates the battlefield. \n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==24)
                    printf("%d. Metallic Crush     | %d    | Metal     | Phy  | Crushes the opponent with a powerful blow of metallic force.\n",i+1,update_power(power[i],player->level));
            else if(q[i]==1&&i==25)
                    printf("%d. Steel Shield       | %d    | Metal     | Phy  | Creates a sturdy shield of steel around the user, increasing both defence and mana resistance by 25%% .\n",i+1,update_power(power[i],player->level));
                    else if(q[i]==1&&i==26)
        printf("%d. Iron Smash         | %d    | Metal     | Phy  | Smashes the foe by fortifying one's resolve with iron-like determination.\n",i+1,update_power(power[i],player->level));
            else
                goto att;

        }
    }
    //for calculating damage
    int damage(int hit_point,int a,int d,int p)
    {
        int dm;
        dm=floor((a*p)/(d*5));
        if(dm>hit_point)
            dm=hit_point;
        if(dm==0)
            dm=1;//the minimum damage is 1
        return dm;
    }
    // for things related to attack
    void attack(int quest[],st *boss,st *player,int power[],float tc[][9])
    {
        dis_att(quest,power,player);
        int ch;
        float effectiveness=1;
        printf("\nEnter your choice: ");
        scanf("%d",&ch);
        for(int i=0;i<9;i++)//for calculating effectiveness of a move
        {
            for(int j=0;j<9;j++)
            {
                if((*player).attribute[i]==1&&(*boss).attribute[j]==1)
                    effectiveness*=tc[i][j];
            }
        }
        if (power[ch-1]>0)//if attack is physical
        {
        (*player).damage=damage((*boss).current_hit_point,(*player).current_attack,(*boss).current_defence,effectiveness*update_power(power[ch-1],(*player).level));
        }
        else if(power[ch-1]<0)//if attack uses mana
        {
            if((*player).current_mana>0)//checking if mana is zero or not
            {
                (*player).damage=damage((*boss).current_hit_point,(*player).current_mana,(*boss).current_mana_resistance,-1*effectiveness*update_power(power[ch-1],(*player).level));
                (*player).current_mana=(*player).current_mana+power[ch-1];
                if((*player).current_mana<0)
                    (*player).current_mana=0;
            }
            else
                (*player).damage=0;
        }
        else//if attacks have non damage effects
        {
            change(player,boss,ch,1);
            (*player).damage=0;
        }
    
    }
    //for boss attacks
    void attack_boss(st *boss,st *player,int power[])
    {   
        int available_attacks[40],counter=0,attack_choice;
        for(int i=0;i<9;i++)
        {
            if(boss->attribute[i]==1)
            {
                for(int j=0;j<3;j++)
                    available_attacks[counter++]=i*3-1+j;
            }
        }
        counter--;
        attack_choice=available_attacks[rand()%counter];
        if(power[attack_choice]>0)
        (*boss).damage=damage((*player).current_hit_point,(*boss).current_attack,(*player).current_defence,update_power(power[attack_choice],boss->level));
        else if(power[attack_choice]<0)
            (*boss).damage=damage((*player).current_hit_point,(*boss).current_mana,(*player).current_mana_resistance,-update_power(power[attack_choice],boss->level));
        else
            change(boss,player,attack_choice,1);
    }
    //for displaying and using items
    void dis_item(int item[],st *f,st *boss)
    {
        int ch;
        items://label for goto
        printf("ITEMS: \n");
        for(int i=0;i<6;i++)
        {

            switch (i)
            {
            case 0:
                if(item[i]>0)
                    printf("%d. Elixir of Vitality : X%d      |restore 10 hit point\n",i+1,item[i]);
                break;
            case 1:
            if(item[i]>0)
                    printf("%d. Potion of Clarity : X%d       | restore 20 mana \n",i+1,item[i]);
                break;
            case 2:
                if(item[i]>0)
                    printf("%d. Potion of Renewal : X%d       | restore 25 hit point\n",i+1,item[i]);
                break;
            case 3:
                if(item[i]>0)
                    printf("%d. Essence of Wisdom : X%d       |restore 50 mana\n",i+1,item[i]);
                break;
            case 4:
                if(item[i]>0)
                    printf("%d. Potion of Revival : X%d       |restore 100 hit point\n",i+1,item[i]);
                break;
            case 5:
                if(item[i]>0)
                    printf("%d. Elixir of Enlightenment : X%d       |restore 100 mana\n",i+1,item[i]);
                break;
        
            default:
                goto items;
                break;
            }
        }

        printf("\nEnter your choice: ");
        scanf("%d",&ch);
        if(item[ch-1]>0)
        {
            change(f,boss,ch,2);//for applying effects of the item
            item[ch-1]-=1;
        }
        else
            return ;
        
        
    }
    //for increases power of attacks as per the level
    int update_power(int pow,int lv)
    {
            pow+=(int)(pow*(lv/100));
            return pow;
    }
    //for increasing level of player
    void update_level(st* player,st*boss,int attrib)
    {
        int original_level=player->level;
        if(boss->experience<(player->level*50-player->experience))
            player->experience+=boss->experience;
        else
        {
            while(boss->experience!=0)
            {
                if(boss->experience<(player->level*50-player->experience))
                {
                    player->experience+=boss->experience;
                    boss->experience=0;
                }
                else
                {
                    boss->experience-=(player->level*50-player->experience);
                    player->experience=0;
                    player->level++;
                }
            }
        }
        if(original_level<player->level)
            level_up(player,player->level-original_level,attrib);
    }
    //for increasing stats after leveling up
    void level_up(st *player,int level_increase,int attrib)
    {
        int increase;
        while(level_increase!=0)
        {
            increase=(int)(player->level/10);
        update_stats(player,player->hit_point+increase,player->attack+increase,player->defence+increase,player->mana+increase,player->mana_resistance+increase,player->speed+increase,player->level++,attrib,player->experience);
            level_increase--;
        }
    }
    //for applying any change in any value of stat
    void change(st *player,st *boss,int choice,int category)
    {
        switch(category)
        {
            case 1://for effects of attack
            switch(choice)
            {
                    case 2:
                        (*player).current_mana*=2;
                        break;
                    case 3:
                        (*player).current_mana_resistance*=2;
                    break;
                    case 5:
                        if( ((*player).current_hit_point+(int)floor((*player).hit_point*0.25)) >(*player).hit_point)
                            (*player).current_hit_point=(*player).hit_point;
                        else
                            (*player).current_hit_point=(*player).current_hit_point+(int)floor((*player).hit_point*0.25);
                        break;
                    case 7:
                        boss->current_defence-=(int)(boss->current_defence*0.25);
                        break;
                    case 9:
                        (*player).current_defence*=2;
                        break;
                    case 12:
                        (*player).current_speed+=(int)((*player).current_speed*0.25);
                        break;
                    case 15:
                        if( ((*player).current_mana+(int)floor((*player).mana*0.25)) >(*player).mana)
                            (*player).current_mana=(*player).mana;
                        else
                            (*player).current_mana=(*player).current_mana+(int)floor((*player).mana*0.25);
                        break;
                    case 18:
                        boss->current_mana_resistance-=(int)(boss->current_mana_resistance*0.25);
                        break;
                    case 21:
                        (*player).current_attack+=(int)((*player).current_attack*0.25);
                        (*player).current_mana+=(int)((*player).current_mana*0.25);
                        break;
                    case 23:
                        if( ((*player).current_hit_point+(int)floor((*player).hit_point*0.5)) >(*player).hit_point)
                            (*player).current_hit_point=(*player).hit_point;
                        else
                            (*player).current_hit_point=(*player).current_hit_point+(int)floor((*player).hit_point*0.5);
                        break;
                    case 26:
                        (*player).current_defence+=(int)((*player).current_defence*0.25);;
                        (*player).current_mana_resistance+=(int)((*player).current_mana_resistance*0.25);
                        break;
                    default :
                    break;
            }
            
                break;
            
            case 2://for effects of items
                if(choice==1)
                {
                    if((*player).current_hit_point+10>(*player).hit_point)
                        (*player).current_hit_point=(*player).hit_point;
                    else
                        (*player).current_hit_point=(*player).current_hit_point+10;
                }
                else if(choice==2)
                {
                    if((*player).current_mana+20>(*player).mana)
                        (*player).current_mana=(*player).mana;
                    else
                        (*player).current_mana=(*player).current_mana+20;
                }
                else if(choice==3)
                {
                    if((*player).current_hit_point+25>(*player).hit_point)
                        (*player).current_hit_point=(*player).hit_point;
                    else
                        (*player).current_hit_point=(*player).current_hit_point+25;
                }
                else if(choice==4)
                {
                    if((*player).current_mana+50>(*player).mana)
                        (*player).current_mana=(*player).mana;
                    else
                        (*player).current_mana=(*player).current_mana+50;
                }
                else if(choice==5)
                {
                    if((*player).current_hit_point+100>(*player).hit_point)
                        (*player).current_hit_point=(*player).hit_point;
                    else
                        (*player).current_hit_point=(*player).current_hit_point+100;
                }
                else if(choice==6)
                {
                    if((*player).current_mana+100>(*player).mana)
                        (*player).current_mana=(*player).mana;
                    else
                        (*player).current_mana=(*player).current_mana+100;
                }
                else;
                break;
            default :
                break;
        } 
    }

    typedef struct _scene {                 // Structure for the scene
        char scene[1000];                   // The text of the scene
        bool option1;                       // If option 1 is available
        bool option2;                       // If option 2 is available
        char option1Text[1000];             // The text of option 1
        char option2Text[1000];             // The text of option 2
        bool kill;                          // If the player is killed
        bool active;                        // If the scene is active
    } Scene;

    char matrix[3][3]; /* the tic tac toe matrix */
    char check(void);                         /* check the board */
    void init_matrix(void);                   /* initialize the matrix */
    void get_player_move(void);               /* get player's move */
    void get_computer_move(void);               /* get computer's move */
    void disp_matrix(void);                    /* display matrix */

    void earth_city() {
        Scene op1[MAX_SCENES];

        // Declaring the text
        strcpy(op1[0].scene,"The empire of Cairo has been suffering severe drought, earthquakes and plague for about a year now. The high priestess tried to contact the heavens for their guidance but all her efforts were in vain. In desperation, the Pharaoh ordered a ritualistic sacrifice of a youth. Unfortunately, you were chosen this time.\nAt the temple during your sacrifice,\nHIGH PRIESTESS: Throw him into the flames of this burning fire..\nYou were thrown into the fire but it was not able to do anything to you.\nYou were summoned to the Prince of the Deity\n");
        strcpy(op1[0].option1Text,"Run from the soldiers");
        strcpy(op1[0].option2Text,"Go with the soldiers ");
        op1[0].active = false;

        strcpy(op1[1].scene,"You are killed by the soldiers");
        strcpy(op1[1].option1Text,"");
        strcpy(op1[1].option2Text,"");
        op1[1].active = false;

        strcpy(op1[2].scene,"At the Prince Chamber:\nPRINCE : Do you know why you are here?");
        strcpy(op1[2].option1Text,"Yes, Because you want to kill me yourself");
        strcpy(op1[2].option2Text,"No, I do not know");
        op1[2].active = false;

        strcpy(op1[3].scene,"HAHA, When the aggressive flames of fire can not kill you, who am I? It is the decision of that great AMUN top keep you alive. It is the decision of that great AMUN top keep you alive. You are the chosen one.would you like to save this kingdom");
        strcpy(op1[3].option1Text,"Yes");
        strcpy(op1[3].option2Text,"No");
        op1[3].active=false;

        strcpy(op1[4].scene,"You are a hero. A person chosen by gods themselves to save our kingdom. please save our kingdom");
        strcpy(op1[4].option1Text,"yes");
        strcpy(op1[4].option2Text,"no I do not think I can do it");
        op1[4].active=false;

        strcpy(op1[5].scene,"Thanks for your help.YOU: Your majesty, please guide me along my way to save our kingdom. The temple on the outskirts of the town has a weird aura surrounding it. Maybe, they are defiled. Let us start by checking upon them");
        strcpy(op1[5].option1Text,"");
        strcpy(op1[5].option2Text,"");
        op1[5].active=false;

        strcpy(op1[6].scene,"Only you were the one who survived the sacrifice. BELIEVE IN YOURSELF. And please save this ancient kingdom of ours YOU: Your majesty, please guide me along my way to save our kingdom. The temple on the outskirts of the town has a weird aura surrounding it. Maybe,   they are defiled. Let us start by checking upon them");
        strcpy(op1[6].option1Text,"");
        strcpy(op1[6].option2Text,"");
        op1[6].active=false;

        int i = 0;              // Variable to keep track of the scene
        int choice;             // Variable to store the choice of the player
        int kill = 0;
        if_killed:              // Label to go back to if the player is killed
        while(i < 7) {          // Loop to go through the scenes
            printf("%s\n", op1[i].scene);
            if (i==6||i==5)
        {
            break;
        }
            if (kill==1)
        {
            break;
        }
            setCyanColor();                                     // Set the color cyan
            printf("Option 1:%s\n",op1[i].option1Text);         // Print the text of option 1
            resetColor();
            setMagentaColor();                                  // Set the color magenta
            printf("Option 2:%s\n", op1[i].option2Text);        // Print the text of option 2
            resetColor();
            scanf("%d", &choice);
            if (i==0&&choice==1)
            {
                i++;
                kill=1;

            }
            
            else if (i==0&&choice==2)
            {
                i+=2;
            }
            else if (i==2&&choice==1)
            {
                i++;
            }
            else if (i==2&&choice==2)
            {
                i+=2;
            }
            else if (i==3&&choice==1)
            {
                i+=2;
            }
            else if (i==3&&choice==2)
            {
                i+=1;
            }
            
            else if (i==4&&choice==1)
            {
                i+=1;
            }
            else if (i==4&&choice==2)
            {
                i+=2;
            }

                    
        }
    bool tic=false;
        char done;
    if (kill!=1){
        printf("when you go to the temple you see some magical text floating.\n");
        printf("the text forms a game of tic tac toe.\n");
        done = ' ';
        init_matrix();
        do {                                            /* Loop To Keep Playing Until Someone Wins or Draw */
            disp_matrix();
            get_player_move();
            done = check(); /* see if winner */
            if (done != ' ') {
                break; /* winner! */
            }
            get_computer_move();
            done = check(); /* see if winner */
        } while (done == ' ');

        if (done == 'X') {
            printf("You won!!!\n");

            tic=true;
        } else if (done == ' ') {
            printf("DRAW\n");
            kill = 1;
        } else {
            printf("you lost !!!!");
            kill = 1;
        }
        disp_matrix(); /* show final positions */
    }
        if (tic==true)
        {
            printf("suddenly a monster attacks you");
    printf("you have to fight the monster\n");
    battle_mechanincs();
    if (won==true){
        printf("You have obtained Eye of Horus\n");
        printf("PRINCE:Now,you have to embark on a journey to save the world and the eye of Horus will help you in your journey\n"); 
        printf("PRINCE:Here is a small gift from my side use it carefully\n");
        printf("Terraforce acquired\n");
        
    }
        }
        char play;
    if (kill == 1 || !won) {
        printf("GAME OVER\n");
        printf("Do you want to play again? (y/n): ");
        
        getchar(); // Consume newline character
    play = getchar();
        if (play == 'y') {
            i = 0;
            kill = 0;
            goto if_killed;
            // Reset other necessary variables here
            // Possibly call a function to reset game state
        }
        else if (play == 'n') {
            printf("Thank you for playing\n");
            exit(0);
        }
       
       
       
        
    }
    
        
    }


    /* Initialize the matrix. */
    void init_matrix(void) {
        int i, j;
        for(i = 0; i < 3; i++) {                 /*Traversing through the Matrix*/
            for(j = 0; j < 3; j++) {
                matrix[i][j] = ' ';             /*Initialising all Elements*/
            }
        }
    }

    /* Get a player's move. */
    void get_player_move(void) {
        int x, y;
        printf("Enter X,Y coordinates for your move: ");
        scanf("%d%*c%d", &x, &y);
        x--; 
        y--;
        if (x < 0 || x > 2 || y < 0 || y > 2 || matrix[x][y] != ' ') {                 
            printf("Invalid move, try again.\n");
            get_player_move(); // Retry move
        } else {
            matrix[x][y] = 'X';
        }
    }

    /* Get a move from the computer. */
    void get_computer_move(void) {
        int i, j;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (matrix[i][j] == ' ') {
                    matrix[i][j] = 'O';
                    return;
                }
            }
        }
    }

    /* Display the matrix on the screen. */
    void disp_matrix(void) {
        int t;
        for (t = 0; t < 3; t++) {
            printf(" %c | %c | %c ", matrix[t][0], matrix[t][1], matrix[t][2]);
            if (t != 2) {
                printf("\n---|---|---\n");
            }
        }
        printf("\n");
    }

    /* See if there is a winner. */
    char check(void) {
        int i;
        for (i = 0; i < 3; i++) {             /* check rows */
            if (matrix[i][0] == matrix[i][1] && matrix[i][0] == matrix[i][2]) {
                return matrix[i][0];
            }
        }
        for (i = 0; i < 3; i++) {      /* check columns */
            if (matrix[0][i] == matrix[1][i] && matrix[0][i] == matrix[2][i]) { 
                return matrix[0][i];
            }
        }
        /* test diagonals */
        if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2]) {
            return matrix[0][0];
        }
        if (matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0]) {
            return matrix[0][2];
        }
        return ' ';         /*No Case Of Winner So Draw*/
    }

    int quiz_game(){                            //function for the quiz game
        int i;
    int ans[NUM_QUESTIONS];
    int points[NUM_QUESTIONS];
    int points_total = 0;                       //initializing the total points
        
    printf("Welcome to the Game\n\n");          //welcome message
    printf("> Press 1 to start the game\n");        //start the game
    printf("> Press 0 to quit the game\n");         //quit the game
        
    scanf("%d", &i);
        
    if (i == 1) {
        printf("The game has started\n\n");
        printf("1) Which god has the head of a jackal\n\n");            //question 1
        printf("1)Anubis\n");
        printf("2)Thoth\n");
        printf("3)Horus\n");
        printf("4)Set\n");
        printf("Enter Your Answer: ");
        scanf("%d", &ans[0]);
        
        if (ans[0] == 1) {
            printf("Correct Answer\n");
            
            points[0] = 5;
            points_total += points[0];
            printf("You have scored %d points\n\n", points[0]);
            
            
        } else {
            printf("Wrong Answer\n");
            points[0] = 0;
        }
        printf("You have scored %d points\n\n", points[0]);             
        printf("2) Who was the first Pharoah\n\n");                     //question 2
        printf("1) Amun ra\n");
        printf("2) Set\n");
        printf("3) Osyrus\n");
        printf("4) Isis\n");
        printf("Enter Your Answer: ");
        scanf("%d", &ans[1]);
        
        if (ans[1] == 3) {
            printf("Correct Answer\n");
            points[1] = 5;
            points_total += points[1];
        } else {
            printf("Wrong Answer\n");
            points[1] = 0;
        }
        printf("You have scored %d points\n\n", points[1]);
        
        printf("3) Who is the wisest of all the gods\n\n");                     //question 3
        printf("1) Geb\n");
        printf("2) Tefnut\n");
        printf("3) Horus\n");
        printf("4) Thoth\n");
        printf("Enter Your Answer: ");
        scanf("%d", &ans[2]);
        
        if (ans[2] == 4) {
            printf("Correct Answer\n");
            points[2] = 5;
            points_total += points[2];
        } else {
            printf("Wrong Answer\n");
            points[2] = 0;
        }
        printf("You have scored %d points\n\n", points[2]);
        
        printf("4) Who is the god of kingship(often depicted with a falcon sitting on a Pharoah shoulders) ?\n\n");                //question 4
        printf("1) Osyrus\n");
        printf("2) Amun Ra\n");
        printf("3) Anubis\n");
        printf("4) Horus\n");
    
        printf("Enter Your Answer: ");
        scanf("%d", &ans[3]);
        
        if (ans[3] == 4) {
            printf("Correct Answer\n");
            points[3] = 5;
            points_total += points[3];
        } else {
            printf("Wrong Answer\n");
            points[3] = 0;
        }
        printf("You have scored %d points\n\n", points[3]);
        printf("5) Who is the ruler of the Underworld?\n\n");               //question 5
        printf("1) Ra\n");
        printf("2) Anubis\n");
        printf("3) Osyrus\n");
        printf("4) Khonshu\n");
            
        printf("Enter Your Answer: ");
        scanf("%d", &ans[4]);
        
        if (ans[4] == 3) {
            printf("Correct Answer\n");
            points[4] = 5;
            points_total += points[4];
        } else {
            printf("Wrong Answer\n");
            points[4] = 0;
        }
        printf("You have scored %d points\n\n", points[4]);
        
        printf("Total Points: %d\n", points_total);
        return points_total;
    } 
    else if (i == 0) {
        printf("The game has ended\n\n");
        
    } 
    else {
        printf("Invalid input\n\n");
    }
        
    }
    void wisdom_city() {
    Scene op1[MAX_SCENES];      //array of structures
        
        strcpy(op1[0].scene,"The eye of horus is in your hands. Thinking what would be its use, you start moving it around. Suddenly, it starts glowing in west direction. When you again pointed it towards north, it darken out. Why did this happen in the direction of wisdom city?");
        op1[0].option1 = true;
        op1[0].option2 = true;
        strcpy(op1[0].option1Text,"Follow the path");
        strcpy(op1[0].option2Text,"Do not follow the path");
        op1[0].active=false;
        
        strcpy(op1[1].scene,"You do not follow the path but the eye of horus glow more brightly and thinking its a sign from gods you decide to follow the path ");
        op1[1].active=false;
        
        strcpy(op1[2].scene,"You saw a statue of sphinx which has a human head and lion body. As you tried to open the gates of the city, it started speaking, Sphinx: Oh young man, stop there, you have to answer my question if you want to enter here!");
        op1[2].option1 = true;
        op1[2].option2 = true;
        strcpy(op1[2].option1Text,"Answer his question ");
        strcpy(op1[2].option2Text,"Go back");
        op1[2].active=false;
        
        strcpy(op1[3].scene,"Sphinx: In shadows I creep, In coffins I sleep. My wine is blood, Few friends do I keep. What am i?\n1. Demons\n2. Vampire\n3. Werewolf\n4. Ghoul");
        op1[3].active=false;
        
        strcpy(op1[4].scene,"Sphinx: Wisdom city allows only Wise ones in it. Here, you are not a perfect fit");
        op1[4].active=false;
        
        strcpy(op1[5].scene,"The gates of the city opened. Suddenly, a baboon came and stole the eye of horus. You followed it towards a big old building. It went inside the building. It was the library of arcanum. Some text appeared on the gates of the building");
        op1[5].active=false;
        
        strcpy(op1[6].scene,"You entered the library. Thoth, the head of wisdom city was sitting there. The eye of horus is in his hands. The baboon is standing beside him. Thoth asks you for the scroll");
        op1[6].option1 = true;
        op1[6].option2 = true;
        strcpy(op1[6].option1Text,"Give him");
        strcpy(op1[6].option2Text,"Deny");
        op1[6].active=false;
        
        strcpy(op1[7].scene,"You give him then scroll. Thoth opens the scroll. Thoth: A great evil resides in an island far away. You need to find Imhotep- the magician who is facing the trials of underworld right now. But you are very weak to enter the underworld. So before entering it you, must have these four skills:\n Terraforce- the one you already have. \nEthicore- it will show you the right path. \nVortex- you can produce tornados of different intensity.\n Aquamansion- Protects from extreme heat and Produce tides of different intensity. \nI will bequeath you the ability of judgement- the Ethicore. You: Where will I get the other two abilities? Thoth: You will get to know yourself, just use the abilities you have");
        op1[7].active=false;
        
        strcpy(op1[8].scene,"You: Give me the eye of horus first. Thoth: First give me the scroll.");
        op1[8].option1 = true;
        op1[8].option2 = true;
        strcpy(op1[8].option1Text,"Give");
        strcpy(op1[8].option2Text,"Deny");
        op1[8].active=false;
        
        strcpy(op1[9].scene,"Thoth gives eye of horus.");
        op1[9].active=false;
        
        strcpy(op1[10].scene,"You give him the scroll. Then he gives you the eye of horus");
        op1[10].active=false;
        
        int i = 0;                          // Variable to keep track of the scene
        int choice = 0;                     // Variable to store the choice of the player
        int question_answer=0;
        while (i < 11) {                    //loop for the game
            printf("%s\n", op1[i].scene);
            if (op1[i].option1 && op1[i].option2) {
                setCyanColor();
                printf("Option 1:%s\n", op1[i].option1Text);
                resetColor();
                setMagentaColor();
                printf("Option 2:%s\n", op1[i].option2Text);
                resetColor();
                scanf("%d",&choice);
            } else if (op1[i].option1) {
                printf("%s\n", op1[i].option1Text);
                scanf("%d",&choice);
            } else if (op1[i].option2) {
                printf("%s\n", op1[i].option2Text);
                scanf("%d",&choice);
            }
            if (i==4)
            {
                i=3;
                continue;
            }
            if (i==3)
            {
                scanf("%d",&choice);
            }
            if(i==7){
            
                break;}
            
    
        if(i==0&&choice==1){
            i+=2;
        }
        else if (i==0&&choice==2)
        {
            i+=1;
        }
        else if (i==1)
        {
            i+=1;
        }
        else if (i==2&&choice==1)
        {
        i+=1;

        }
        else if (i==2&&choice==2)
        {
            i=2;
        }
        else if (i==3&&choice==2)
        {
            i+=2;

        }
        else if (i==3&&choice!=2)
        {
            i+=1;
        }
        else if (i==5)
        {
            printf("the texts are appearing to be asking something\n");
            printf("you have to answer the questions to enter the library\n");
            
        int points=quiz_game();
        if(points==25)
        {
            i+=1;
        }
        else
        {
            printf("you are transported to the outside of the city\n");
            i=4;
        }
        }
        else if(i==6){
            if(choice==1)
            {
                i=7;
                
            }
            else
            {
                i=8;
            }
        }
        else if(i==8){
            if(choice==1)
            {
                i=7;
                
            }
            else
            {
                printf("baboon attacks");
                battle_mechanincs();
                if(won==true)
                {
                    i=10;
                }
                else
                {
                    i=9;
                }
            }
        }
        else if(i==10){
            i=7;
            
            }
        else if (i==9)
        {
            i=7;
        }
        
        else
        {
            printf("Invalid Choice\n");
        }
        

    }

    }

    void  water_city() {
    Scene op1[MAX_SCENES];                         
    //Declaring the text
    strcpy(op1[0].scene,"The Nile is the enterance to the city of Noor, presided over by the goddess tefnut. When you reach near the location to where the eye of Horus was pointing the water retracts, the city appears and you enter the city.\nIn the market: ");
    strcpy(op1[0].option1Text,"Option1: Talk to guards");
    strcpy(op1[0].option2Text,"Option2: Talk to a merchant");
    op1[0].active = false;
    strcpy(op1[1].scene,"You are arrested by the guards for looking too suscpicious...In the prision you meet a few people in your cell who seem distressed so you start a conversation ");
    strcpy(op1[1].option1Text,"Option1: What happened here? Why did they takeover? And why is the old king in prison?");
    strcpy(op1[1].option2Text,"Option2: What's going on? And where is the king? Why am I arrested? ");
    op1[1].active = false;
    strcpy(op1[2].scene,"*Fugitive 2:* The regime staged a military coup, claiming the old king was corrupt. But in reality, they're the corrupt ones, using their power for personal gain.");
    strcpy(op1[2].option1Text,"Option1: That's horrible. How can they get away with it?");
    strcpy(op1[2].option2Text,"Option2: Interesting a full military coup!!");
    op1[2].active = false;
    strcpy(op1[3].scene,"*Fugitive 3:* They control everything now, the military, the courts. Anyone who speaks out against them is silenced.");
    strcpy(op1[3].option1Text,"Option1: But what can we do? How can we fight back against such tyranny?");
    strcpy(op1[3].option2Text,"Option2: Is there any way I can help?");
    op1[3].active = false;
    strcpy(op1[4].scene,"*Fugitive 5:* It will be dangerous, no doubt. But if we don't stand up for what's right, we'll be living in fear forever, I will show you the secret passage to the prison where the king is kept, we must go there and help him escape.");
    strcpy(op1[4].option1Text,"Option1: Why don't you guys eacape?");
    strcpy(op1[4].option2Text,"Option2: what about you?");
    op1[4].active = false;
    strcpy(op1[5].scene,"*Fugitive 1:* were gonna escape from the other side , just take care of the king ");
    strcpy(op1[5].option1Text,"Option1: Okay!");
    strcpy(op1[5].option2Text,"Option2: Fine then...");
    op1[5].active = false;
    strcpy(op1[6].scene,"Merchant- Haven't you heard there was a coup sometime ago. The military seize the control of the palace and threw the king in the dungeon. Pretty corrupt those military folks are, selling the publics rations to foreigners for high profit.");
    strcpy(op1[6].option1Text,"Option1: wait what?");
    strcpy(op1[6].option2Text,"Option2: A military coup!!!");
    op1[6].active = false;
    strcpy(op1[7].scene,"Merchant: that too when our goddess Tefnut is missing, and our country is facing severe drought.\nThe guard noticed you talking to the merchants and heard that you are detecting the trying to find out the secrets of the city.He charged towards you.\nGuard: I have never seen you in this city before. Where are you from?");
    strcpy(op1[7].option1Text,"Option1: The earth city...");
    strcpy(op1[7].option2Text,"Option2: why should I tell you?"); 
    op1[7].active = false; 
    strcpy(op1[8].scene,"The guard commands to handcuff you.");
    strcpy(op1[8].option1Text,"Option1: Punch him and run");
    strcpy(op1[8].option2Text,"Option2: Run away");
    op1[8].active = false;
    strcpy(op1[9].scene,"While you were running a merchant pointed you towards a small dark place.");
    strcpy(op1[9].option1Text,"Option1: Ignore him and continue running");
    strcpy(op1[9].option2Text,"Option2: continue with the merchant");
    op1[9].active = false;
    strcpy(op1[10].scene,"You tripped and fell, you got caught.");
    strcpy(op1[10].option1Text,"Option1: leave me alone!!!");
    strcpy(op1[10].option2Text,"Option2: Why are you arresting me...");
    op1[10].active = false;
    strcpy(op1[11].scene,"You get to a place where there are a number of fugitives.");
    strcpy(op1[11].option1Text,"Option1: Thank you for saving me back there. I didn't know who else to turn to.");
    strcpy(op1[11].option2Text,"Option2: What was that? I mean, I could've handled that myself but thanks anyway.");
    op1[11].active = false;
    strcpy(op1[12].scene,"*Fugitive 1:* No need to thank us, kid. We've been in your shoes. We know what it's like to be hunted by the regime.");
    strcpy(op1[12].option1Text,"Option1: What happened here? Why did they take over? And why is the old king in prison?");
    strcpy(op1[12].option2Text,"Option2: What's going on? And where is the king?");
    op1[12].active = false;
    strcpy(op1[13].scene,"*Fugitive 2:* The regime staged a military coup, claiming the old king was corrupt. But in reality, they're the corrupt ones, using their power for personal gain.");
    strcpy(op1[13].option1Text,"Option1: That's horrible. How can they get away with it?");
    strcpy(op1[13].option2Text,"Option2: Interesting a full military coup!!!");
    op1[13].active = false; 
    strcpy(op1[14].scene,"*Fugitive 3:* They control everything now the military, the courts. Anyone who speaks out against them is silenced.");
    strcpy(op1[14].option1Text,"Option1: But what can we do? How can we fight back against such tyranny?");
    strcpy(op1[14].option2Text,"Option2: Is there any way I can help?");
    op1[14].active = false; 
    strcpy(op1[15].scene,"*Fugitive 5:* It will be dangerous, no doubt. But if we don't stand up for what's right, we'll be living in fear forever, I will show you the secret passage to the prison where the king is kept, we must go there and help him escape.");
    strcpy(op1[15].option1Text,"Option1: I will go alone, don't worry you guys stay here, for I am the chosen one");
    strcpy(op1[15].option2Text,"Option2: Leave it to me… you guys just protect the rations for the common folk");
    op1[15].active = false; 
    strcpy(op1[16].scene,"*Fugitive 6:* You have a brave heart, kid. With your courage and our experience, we can work together to bring down this corrupt regime and restore the old king to his rightful place.");
    strcpy(op1[16].option1Text,"Option1: Then let's do it. Let's fight for justice and freedom!");
    strcpy(op1[16].option2Text,"Option2: Let's go!");
    op1[16].active = false;
    strcpy(op1[17].scene,"At the king's cell\nKing: Who are you, and why are you here?");
    strcpy(op1[17].option1Text,"Option1: Your Majesty, I am but a traveler, when I came to your kingdom I couldn't help but notice the harsh effects of the new corrupt regime, hence I joined a group of your loyalist fugitives. The fugitives brought me here, saying you needed help to escape.");
    strcpy(op1[17].option2Text,"Option2: Just your savior!, I came here to help you!");
    op1[17].active = false;
    strcpy(op1[18].scene,"King: Escape? Ah, I see. Sadly, it's not possible for me to leave this cell. The guards are too vigilant, and the risk is too great.");
    strcpy(op1[18].option1Text,"Option1: But your majesty I can't leave you here, the danger is too grave.");
    strcpy(op1[18].option2Text,"Option2: But, your highness, what about you?");
    op1[18].active = false;
    strcpy(op1[19].scene,"King: Please try to understand, you are our only hope we can't afford to loose you");
    strcpy(op1[19].option1Text,"Option1: What me?");
    strcpy(op1[19].option2Text,"Option2: How'd you know");
    op1[19].active = false;
    strcpy(op1[20].scene,"King: I can clearly see the eye of Horus around your neck, you are the chosen one, you must be safe ");
    strcpy(op1[20].option1Text,"Option1: I understand, Your Majesty. Is there anything else I can do for you?");
    strcpy(op1[20].option2Text,"Option2: Fine, but is there anyway else I can help you");
    op1[20].active = false;
    strcpy(op1[21].scene,"King: Actually, there is. Our kingdom's god Tefnut has gone missing. Without her guidance, our people are lost. I need you to find her. This ancient scroll contains clues that will lead you to her.");
    strcpy(op1[21].option1Text,"Option1: I will do my best, Your Majesty. I won't let you down");
    strcpy(op1[21].option2Text,"Option2: Count on me!");
    op1[21].active = false;
    strcpy(op1[22].scene,"King: I have faith in you, young one. May the gods be with you on your journey.\nYou met with the fugitives and told them about the conversation with the king and bid them farewell.\nThe scroll contained some indecipherable script yet one part of it was readable, \nIt was a spell to cast the Devine 'Aquamansion'\n'Aquamansion acquire'\nHmm wait where should I go now");
    strcpy(op1[22].option1Text,"Option1: Wind city");
    strcpy(op1[22].option2Text,"Option2: Wind city");
    op1[22].active = false;
    int i = 0;
    int choice,normal,merchant,cought;
    normal = 0;
    merchant = 0;
    cought = 0;
    while(i < 23){
    printf("%s\n",op1[i].scene);
    setCyanColor();
    printf("%s\n",op1[i].option1Text);
    resetColor();
    setMagentaColor();
    printf("%s\n",op1[i].option2Text);
    resetColor();
    scanf("%d",&choice);
    if (i == 0 && choice == 1){
    normal = 1;
    }
    if (i == 0 && choice == 2){
    i = 5;
    merchant = 1;
    }
    if(i == 5 && normal){
    i = 16;
    }
    if(i == 9 && choice == 1){
    cought = 1;
    }
    if(i == 9 && choice == 2){
    i = 10;
    }
    if(i== 10 && cought){
    i = 0;
    }
    if(i==5 && cought){
    i =16;
    }
    i++;
    }

    }




    bool woncasino=false;           //variable to check if the player won the casino game

    int rules1();                   //function to display the rules of the casino game
    int round1();                   //function to play the casino game
    void casinogame() {
        rules1();
        printf("\n\n");
        round1();
    }

    int round1() {
        printf("\n\n");
        int deposit1;                   //variable to store the deposit amount
        printf("Enter the amount of money to deposit for the game : $");
        scanf("%d", &deposit1);
        printf("\n\n");
        printf("Your current balance is $%d", deposit1);
        printf("\n\n");
        int bet1;                       //variable to store the bet amount
        printf("Enter the money to bet : $");
        scanf("%d", &bet1);
        printf("\n\n");
        if (bet1 > deposit1) {          //checking if the bet amount is greater than the deposit amount
            printf("Money Exceeds,\n");
            printf("Enter Again : $");
            scanf("%d", &bet1);
        }
        printf("\n\n");
        int computer1 = 2;              //variable to store the winning number
        int guess1;
        printf("Guess a number from 1 to 10 : ");
        scanf("%d", &guess1);           //variable to store the guessed number
        printf("\n\n");
        if (guess1 > 10) {
            printf("Number Exceeds,\n");
            printf("Enter Again : ");
            scanf("%d", &guess1);
        }
        printf("\n\n");
        if (guess1 == computer1) {
            printf("You have won $%d", bet1);
            woncasino = true;           //setting the variable to true if the player wins
            printf("\n\n");
            int bet2 = bet1 * 10;
            int new_won = (bet1 == deposit1) ? (bet2 + 0) : (bet2 + deposit1);
            printf("You have won $%d as total", new_won);
        } else if (guess1 != computer1) {
            printf("You have loss $%d", bet1);
            printf("\n\n");
            printf("The correct number is : %d", computer1);
            printf("\n\n");
            int new_los = deposit1 - bet1;
            printf("Now you have $%d as a balance", new_los);
            printf("\n\n");
            if (new_los == 0) {
                printf("Sir,you have $0 as a balance\n");
                printf("You have no more money to play\n");
                printf("Have a nice day");
            } else {
                int decision;
                printf("Press 1 for YES\n");
                printf("Press 2 for NO\n\n");
                printf("Do you want continue(Y/N) : ");
                scanf("%d", &decision);
                printf("\n\n");
                if (decision == 1) {
                    rules1();
                    printf("\n\n");
                    printf("Now you have $%d as a balance", new_los);
                    printf("\n\n");
                    int bet3;
                    printf("Enter the money to bet : $");
                    scanf("%d", &bet3);
                    printf("\n\n");
                    if (bet3 > new_los) {
                        printf("Money Exceeds,\n");
                        printf("Enter Again : $");
                        scanf("%d", &bet3);
                    }
                    int guess2;
                    printf("Enter a number from 1 to 10 : ");
                    scanf("%d", &guess2);
                    printf("\n\n");
                    int computer2 = computer1;              //variable to store the winning number
                    if (guess2 > 10) {
                        printf("Number Exceeds,\n");
                        printf("Enter Again : $");
                        scanf("%d", &guess2);
                        printf("\n\n");
                    }
                    if (guess2 == computer2) {
                        printf("You have won $%d", bet3);
                        printf("\n\n");
                        int b = bet3 * 10;
                        int b1 = (bet3 == new_los) ? (b + 0) : (b + new_los);
                        printf("You have won $%d as total\n", b1);
                    } else if (guess2 != computer2) {
                        printf("You have loss $%d\n", bet3);
                        printf("\n\n");
                        int b2 = new_los - bet3;
                        printf("You have $%d as a total\n", b2);
                        printf("\n\n");
                        if (b2 == 0) {
                            printf("Sir,you have $0 as a balance\n");
                            printf("You have no more money to play\n");
                            printf("Have a nice day");
                        }
                    }
                } else if (decision == 2) {
                    printf("Have a good day sir");
                }
            }
        }
        
        }

    int rules1() {
        printf("----------------------------------------------------------------------------\n");
        printf("                         RULES\n");
        printf("-----------------------------------------------------------------------------\n");
        printf("         1. Choose any number between 1 to 10\n");
        printf("         2. If you win you will get 10 times of money you bet\n");
        printf("         3. if you bet on wrong number you will lose your betting amount\n");
        printf("------------------------------------------------------------------------------\n");
        return 0;
    }
    

int crossword(){
    int NA=0,C=0,W=0;//NA->Number of ques not attempted;W->Wrong;C->Correct
    //preparing the crossword in the form of a 2D array
    char crossword[8][8];
    crossword[0][0]='C';crossword[0][1]='I';crossword[0][2]='I'; crossword[0][3]='T';crossword[0][4]='M';crossword[0][5]='S';
    crossword[0][6]='T';crossword[0][7]='F';crossword[1][0]='P';crossword[1][1]='O';crossword[1][2]='P';crossword[1][3]='S';
    crossword[1][4]='U'; crossword[1][5]='O';crossword[1][6]='I';crossword[1][7]='O'; crossword[2][0]='A'; crossword[2][1]='M';
    crossword[2][2]='M'; crossword[2][3]='C'; crossword[2][4]='E';crossword[2][5]='Q'; crossword[2][6]='F'; crossword[2][7]='R';
    crossword[3][0]='T';crossword[3][1]='D';crossword[3][2]='A';crossword[3][3]='P'; crossword[3][4]='N'; crossword[3][5]='L';
    crossword[3][6]='R'; crossword[3][7]='T'; crossword[4][0]='N';crossword[4][1]='B'; crossword[4][2]='H'; crossword[4][3]='R';
    crossword[4][4]='I';crossword[4][5]='N';crossword[4][6]='A';crossword[4][7]='R';crossword[5][0]='A';crossword[5][1]='E';
    crossword[5][2]='L';crossword[5][3]='B';crossword[5][4]='A';crossword[5][5]='L';crossword[5][6]='C';crossword[5][7]='A';
    crossword[6][0]='I';crossword[6][1]='D';crossword[6][2]='E';crossword[6][3]='W';crossword[6][4]='C';crossword[6][5]='P';
    crossword[6][6]='E';crossword[6][7]='N';crossword[7][0]='M';crossword[7][1]='S';crossword[7][2]='E';crossword[7][3]='X';
    crossword[7][4]='C';crossword[7][5]='E';crossword[7][6]='L';crossword[7][7]='R';
    printf("WELCOME TO THE GAME!!!!\n");
    char reply;
    printf("PRESS 'A' IF YOU WANNA PLAY ELSE 'Z'TO QUIT\n");
        getchar(); // Consume newline character
reply = getchar();
    
    if(reply=='A'){
        //do nothing and continue with the game
    }
    else if(reply=='Z'){
        printf("YOU ARE OUT OF GAME NOW\n");    //quit from the game
        return 0;
    }
    else{
        printf("Invalid reply:\n");
        return 0;
    }
    printf("SOME RULES OF THE GAME:\n");
    printf("1. 5 POINTS WILL BE AWARDED FOR EACH CORRECT ANSWER\n");
    printf("2. 2 POINTS WILL BE DEDUCTED FOR EACH WRONG ANSWER\n");
    printf("3. 1 POINT WILL BE DEDUCTED FOR NOT ATTEMPTING THE QUESTION\n");
    printf("4. YOU CAN ANSWER BY ENTERING ROW AND COLUMN NUMBER OF BEGINNING AND ENDING OF THAT WORD\n");
    printf("5. NUMBERING STARTS AS USUAL FROM 1\n");
    printf("                 SO LETS BEGIN ENTHUSIASTS                \n");
    
    printf("                 YOUR CROSSWORD IS READY!!!!\n"                   );
    printf("**SEARCH WORDS FROM HERE AND ANSWER THE FOLLOWING QUESTIONS**\n");
    for(int i=0;i<8;i++){                         //printing the crossword*********
        for(int j=0;j<8;j++){
            printf("%c\t",crossword[i][j]);
        }
        printf("\n");
    }
    int rb,cb,re,ce;//rb->row no. of beginning  ce->column no. of ending  and so on....
    int score=0;//variable for calculating total score
    int static answer;//variable to decide whether one will play or leave the question
    printf("          SO HERE COMES THE FIRST QUESTION    \n         ");
    printf("Q1. WHAT IS THAT SOFTWARE PROGRAM CALLED THAT TRANSLATES HIGH LEVEL LANGUAGE TO LOW LEVEL LANGUAGE?\n");
    printf("PRESS '1' TO ANSWER ELSE '0' \n");//1->PLAY  0->NEXT
    scanf("%d",&answer);
    if(answer==1){
        printf("Enter beginning row number:\n");
        scanf("%d",&rb);
        printf("Enter beginning column number:\n");
        scanf("%d",&cb);
        printf("Enter ending row number:\n");
        scanf("%d",&re);
        printf("Enter ending column number\n");
        scanf("%d",&ce);
        if(rb==1 &&cb==1 &&re==8 &&ce==8 ){
            printf("YES YOU ARE CORRECT\n");
            printf("CORRECT ANSWER IS COMPILER\n");
            score=score+5;
            C++; //increase number of correctly answered questions by 1
        }
        else{
            printf("OOPS THIS IS NOT THE CORRECT ANSWER:\n");
            printf("CORRECT ANSWER IS COMPILER\n");
            score=score-2;
            W++;  //increase number of wrongly answered questions by 1
        }
    }
    else if(answer==0){
        score=score-1;
        NA++;     //increase the number of unattempted questions by 1
    }
    printf("CURRENTLY YOUR TOTAL SCORE IS:%d\n",score);
    printf("CURRENTLY NUMBER OF CORRECTLY ANSWERED QUESTIONS ARE :%d ::WRONG NUMBER OF QUESTIONS ARE:%d ::NUMBER OF UNATTEMPTED QUESTIONS ARE:%d\n",C,W,NA);
        printf("Q2. What is the name of the first calculating device?\n");
        printf("PRESS '1' TO ANSWER ELSE '0' \n");//1->PLAY  0->NEXT
        scanf("%d",&answer);
        if(answer==1){
            printf("Enter beginning row number:\n");
            scanf("%d",&rb);
            printf("Enter beginning column number:\n");
            scanf("%d",&cb);
            printf("Enter ending row number:\n");
            scanf("%d",&re);
            printf("Enter ending column number\n");
            scanf("%d",&ce);
            if(rb==6 &&cb==1 &&re==1 &&ce==6 ){
                printf("YES YOU ARE CORRECT\n");
                printf("CORRECT ANSWER IS ABACUS\n");
                score=score+5;
                C++;    //increase number of correctly answered questions by 1
            }
            else{
                printf("OOPS THIS IS NOT THE CORRECT ANSWER:\n");
                printf("CORRECT ANSWER IS ABACUS\n");
                score=score-2;
                W++;    //increase number of wrongly answered questions by 1
            }
        }
        else if(answer==0){
            score=score-1;
            NA++;      //increase the number of unattempted questions by 1
        }
        printf("CURRENTLY YOUR TOTAL SCORE IS:%d\n",score);
        printf("CURRENTLY NUMBER OF CORRECTLY ANSWERED QUESTIONS ARE :%d ::WRONG NUMBER OF QUESTIONS ARE:%d ::NUMBER OF UNATTEMPTED QUESTIONS ARE:%d\n",C,W,NA);
        // printf("Enter A to continue Z to exit from game:");
        printf("Q3. DO YOU KNOW THE NAME OF THE FIRST SUPERCOMPUTER IN INDIA?\n");
        printf("PRESS '1' TO ANSWER ELSE '0' \n");//1->PLAY  0->NEXT
        scanf("%d",&answer);
        if(answer==1){
            printf("Enter beginning row number:\n");
            scanf("%d",&rb);
            printf("Enter beginning column number:\n");
            scanf("%d",&cb);
            printf("Enter ending row number:\n");
            scanf("%d",&re);
            printf("Enter ending column number\n");
            scanf("%d",&ce);
            if(rb==7 &&cb==6 &&re==3 &&ce==2){
                printf("YES YOU ARE CORRECT\n");
                printf("CORRECT ANSWER IS PARAM\n");
                score=score+5;
                C++;     //increase number of correctly answered questions by 1
            }
            else{
                printf("OOPS THIS IS NOT THE CORRECT ANSWER:\n");
                printf("CORRECT ANSWER IS PARAM\n");
                score=score-2;
                W++;     //increase number of wrongly answered questions by 1
            }
        }
        else if(answer==0){
            score=score-1;
            NA++;        //increase the number of unattempted questions by 1
        }
        printf("CURRENTLY YOUR TOTAL SCORE IS:%d\n",score);
        printf("CURRENTLY NUMBER OF CORRECTLY ANSWERED QUESTIONS ARE :%d ::WRONG NUMBER OF QUESTIONS ARE:%d ::NUMBER OF UNATTEMPTED QUESTIONS ARE:%d\n",C,W,NA);
        printf("Q4. WHAT IS THE NAME OF FIRST PROGRAMMING LANGUAGE?\n");
        printf("PRESS '1' TO ANSWER ELSE '0' \n");//1->PLAY  0->NEXT
        scanf("%d",&answer);
        if(answer==1){
            printf("Enter beginning row number:\n");
            scanf("%d",&rb);
            printf("Enter beginning column number:\n");
            scanf("%d",&cb);
            printf("Enter ending row number:\n");
            scanf("%d",&re);
            printf("Enter ending column number\n");
            scanf("%d",&ce);
            if(rb==1 &&cb==8 &&re==7 &&ce==8){
                printf("YES YOU ARE CORRECT\n");
                printf("CORRECT ANSWER IS FORTRAN\n");
                score=score+5;
                C++;    //increase number of correctly answered questions by 1
            }
            else{
                printf("OOPS THIS IS NOT THE CORRECT ANSWER:\n");
                printf("CORRECT ANSWER IS FORTRAN\n");
                score=score-2;
                W++;    //increase number of wrongly answered questions by 1
            }
        }
        else if(answer==0){
            score=score-1;
            NA++;        //increase the number of unattempted questions by 1
        }
        printf("CURRENTLY YOUR TOTAL SCORE IS:%d\n",score);
        printf("CURRENTLY NUMBER OF CORRECTLY ANSWERED QUESTIONS ARE :%d ::WRONG NUMBER OF QUESTIONS ARE:%d ::NUMBER OF UNATTEMPTED QUESTIONS ARE:%d\n",C,W,NA);
        printf("!!!!!!AND NOW COMES THE LAST QUESTION OF THIS SHORT QUIZ!!!!!!\n");
        printf("Q5. WHAT IS THE NAME OF THE WORLD'S FIRST SUCCESSFUL ELECTRONIC COMPUTER:\n");
        printf("PRESS '1' TO ANSWER ELSE '0' \n");//1->PLAY  0->NEXT
        scanf("%d",&answer);
        if(answer==1){
            printf("Enter beginning row number:\n");
            scanf("%d",&rb);
            printf("Enter beginning column number:\n");
            scanf("%d",&cb);
            printf("Enter ending row number:\n");
            scanf("%d",&re);
            printf("Enter ending column number\n");
            scanf("%d",&ce);
            if(rb==3 &&cb==5 &&re==7 &&ce==5){
                printf("YES YOU ARE CORRECT\n");
                printf("CORRECT ANSWER IS ENIAC\n");
                score=score+5;
                C++;    //increase number of correctly answered questions by 1
            }
            else{
                printf("OOPS THIS IS NOT THE CORRECT ANSWER:\n");
                printf("CORRECT ANSWER IS ENIAC\n");
                score=score-2;
                W++;   //increase number of wrongly answered questions by 1
            }
        }
        else if(answer==0){
            score=score-1;
            NA++;      //increase the number of unattempted questions by 1
        }
        printf("CURRENTLY YOUR TOTAL SCORE IS:%d\n",score);
        printf("CURRENTLY NUMBER OF CORRECTLY ANSWERED QUESTIONS ARE :%d ::WRONG NUMBER OF QUESTIONS ARE:%d ::NUMBER OF UNATTEMPTED QUESTIONS ARE:%d\n",C,W,NA);
        srand(time(NULL));
        printf("TILL NOW HIGHEST SCORE WAS:%d\n",(rand()%(26-20))+20);//generates a random highest score between 20-25
        printf("YOUR SCORE IS :%d\n",score);
        if(score<=25 && score >=18){
            printf("NICELY PLAYED!!!!\n");
        }
        if(score<=17 && score>=10){
            printf("WELL TRIED!!!!\n");
        }
        if(score<=9){
            printf("BETTER LUCK NEXT TIME!!!!\n");
        }
        return score;
}

int strcicmp(char *a,char *b) {                     //function to compare two strings
    while (*a && *b) {
        if (tolower((char)*a) != tolower((char)*b)) {
            return tolower((char)*a) - tolower((char)*b);
        }
        a++;
        b++;
    }
    return tolower((char)*a) - tolower((char)*b);
}
void rules()                                            //function to display the rules of the game
{
	printf("  J  U  P  I  T  E  R  \n  H  E  N  O  O  U  A  \n  I  U  I  A  K  R  Z  \n  S  R  L  T  Y  O  E   \n  T  O  E  G  O  L  D   \n  O  C  U  R  I  E  U   \n  R  H  Y  T  H  M  S  \n  Y  C  E  D  A  R  K\n");
    printf("  The Rules Of The Game are:\n");
    printf("  1. You have to answer the questions given based on the crossword provided.\n");
    printf("  2. You will be awarded 5 points for each correct answer.\n");
    printf("  3. For each wrong answer, 1 point will be deducted.\n");
    printf("  4. If you do not attempt a question, no marks will be reduced.\n");
    printf("  5. You need to give an answer if you know it; this will be counted as an attempted question.\n");
    printf("  6. For questions not answered, input 'NA'.\n");
    printf("  7. enter \"HINT\" if you want a hint .\n");
    printf("  8. After using a hint only 3 points will be given for that particular question if answered correctly \n     2 marks will be deducted if still not attempted or if the answer given is incorrect .\n");
    
}
int puzzle() {                              //function to play the game
	rules();
    char* ques[] = {
        "Currency of the European nation",
        "Smallest unit of life",
        "Capital city of Japan",
        "Study of past as it is described in written documents",
        "An English term for demolished or destroyed",
        "Largest planet of the Solar System",
        "Largest river of the country Egypt",
        "First woman to win a Nobel Prize",
        "Time just before night when daylight has gone and it's not completely dark",
        "A regular repeated pattern of sound or movement",
        "Coniferous trees native to the Himalayas"
    };
    char* ans[] = {                 //array of answers
        "EURO",
        "CELL",
        "TOKYO",
        "HISTORY",
        "RAZED",
        "JUPITER",
        "NILE",
        "CURIE",
        "DUSK",
        "RHYTHMS",
        "CEDAR"
    };
     char* hints[] = {                  //array of hints
        "The hints are \n 1. EURO \n 2. POUND \n 3. DOLLAR \n 4. DINAR \n",
        "The hints are \n 1. CELL \n 2. MITOCHONDRIA \n 3. TISSUE \n 4. CELL WALL \n",
        "The hints are \n 1. TOKYO \n 2. OSAKA \n 3. KYOTO \n 4. KOBE \n",
        "The hints are \n 1. HISTORY \n 2. PHILOSOPHY \n 3. SURVEY  \n 4. CASE STUDY \n",
        "The hints are \n 1. DISMANTLE \n 2. RAZED \n 3. WRECKED  \n 4. SHATTERED \n",
        
        "The hints are \n 1. MARS \n 2. PLUTO \n 3. JUPITER  \n 4. SATURN \n",
        "The hints are \n 1. LENA \n 2. YELLOW \n 3. AMAZON  \n 4. NILE \n",
        "The hints are \n 1. CURIE \n 2. GRACE \n 3. SALLY  \n 4. MARIA \n",
        "The hints are \n 1. DAWN \n 2. DUSK \n 3. TWILIGHT  \n 4. EVENING \n",
        "The hints are \n 1. RHYTHMS \n 2. PATTERN \n 3. STRESS  \n 4. FLOW \n",
        "The hints are \n 1. FIR \n 2. PINE \n 3. CEDAR  \n 4. CYPRUS \n"
    };

    char anss[100]; // Allocate memory for the answer
    int points = 0;

    // Display questions
    printf("\nAnswer the questions:\n");
    int i=0;
    for ( i = 0; i < 11; ++i) {
        printf("%d. %s: ", i + 1, ques[i]);
        scanf("%s", anss); // Read the answer

        // Check if the user asked for a hint
        if (strcicmp(anss, "HINT") == 0) {
            printf("%s", hints[i]);
            printf("  Now give the correct answer \n ");
            scanf("%s", anss); // Read the answer again
            if (strcicmp(anss, ans[i]) == 0) {
                points += 3; // Correct answer after hint
            } else {
                points -= 2; // Incorrect answer or not attempted after hint
            }
        } else if (strcicmp(anss, ans[i]) == 0) {
            points += 5; // Correct answer without hint
        } else if (strcicmp(anss, "NA") != 0) {
            points -= 1; // Incorrect answer and not 'NA'
        }
    }
    printf("\nThe total points scored is: %d\n", points);
    return points;
}



    void wind_city(){                       //function for the wind city
        Scene op1[MAX_SCENES];              //array of structures to store the scenes
        strcpy(op1[0].scene,"It is a city full of hustle and bustle.  It is a metropolitan and the biggest city in the world. In the heart of city lies the royal casino. The casino is known for its games and the money that flows in it but it also has a secret. Those who can win it can get the  vortex required for your quest");

        strcpy(op1[0].option1Text,"Enter the casino and bet");
        strcpy(op1[0].option2Text,"Enter the casino and steal the vortex");
        op1[0].option1=true;
        op1[0].option2=true;
        op1[0].active=false;
        strcpy(op1[1].scene,"You entered the casino and tried to steal the vortex but you were caught by the guards and thrown out of the city");
        op1[1].active=false;
        strcpy(op1[2].scene,"You entered the casino and bet. ");
        op1[2].option1=false;
        op1[2].option2=false;
        strcpy(op1[3].scene,"\nYou won the game and got the vortex");
        op1[3].active=false;
        strcpy(op1[4].scene,"You lost the game and were thrown out of the city");
        op1[4].active=false;
        strcpy(op1[5].scene,"You have to go the wisdom city to meet Thoth.When you arrive in wisdom city and go to Thoth's library you see 2 puzzles both different from the previous one. ");
        op1[5].active=false;
        strcpy(op1[6].scene,"You have cleared both puzzles ");
        op1[6].active=false;
        strcpy(op1[7].scene,"You have failed to clear the puzzles , you had to be smarter than that");
        op1[7].active=false;
        int i = 0;                //variable to store the scene number
        int choice = 0;             //variable to store the choice             
        if_lost:                    //label to go back to the start of the game if lost
        while (i < 8) {
            printf("%s\n", op1[i].scene);
            if (i==4||i==6||i==7)
            {
                break;
            }
            if (i==1)
            {
                break;
            }
            if (op1[i].option1 && op1[i].option2) {
                setCyanColor();
                printf("Option 1:%s\n", op1[i].option1Text);
                resetColor();
                setMagentaColor();
                printf("Option 2:%s\n", op1[i].option2Text);
                resetColor();
                scanf("%d",&choice);
            } else if (op1[i].option1) {
                printf("%s\n", op1[i].option1Text);
                scanf("%d",&choice);
            } else if (op1[i].option2) {
                printf("%s\n", op1[i].option2Text);
                scanf("%d",&choice);
            }
            if(i==0&&choice==1){
                i=2;
            }
            else if(i==0&&choice==2){
                i=1;
                
            }
            else if (i==2)
            {
                casinogame();
                if(woncasino==true)
                {
                    i=3;
                }
                else
                {
                    i=4;
                }
            }
            else if (i==3)
            {
                i=5;
            }
            else if (i==5)
            {
                int points_puzzle=puzzle();
                if(points_puzzle>=50)
                {
                    int points_crossword=crossword();
                    if (points_crossword>=18)
                    {
                        i=6;
                    }
                    else
                    {
                        i=7;
                    }
                    
                    
                }
                else
                {
                    i=7;
                }
            }
            
            
            else
            {
                printf("Invalid Choice\n");
            }
            
            
        }
    if (i==4||i==7||i==1)
    {
        i=0;
        printf("Do you want to try again\n ");
        printf("Press 1 for YES\n");
        printf("Press 2 for NO\n\n");
        int cho;
        scanf("%d",&cho);
        if(cho==1)
        {
        goto if_lost;}
        else
        {
            printf("You have lost the game\n");
            exit(0);
        }
    }


    }


    void underworld() {
        Scene op1[50];              //array of structures to store the scenes
        strcpy(op1[0].scene,"After Gathering all the abilities you go back to meet up with Thoth\nThoth: I see you have successfully gathered all the abilities I recommended.");
        strcpy(op1[0].option1Text,"Option1: Yes sir!");
        strcpy(op1[0].option2Text,"Option2: Oh finally!");

        strcpy(op1[1].scene,"Thoth: Now that you are ready, you must go to the underworld at once, and bring Imhotep back to the land of the living.");
        strcpy(op1[1].option1Text,"Option1: But why do I need to bring Imhotep?");
        strcpy(op1[1].option2Text,"Option2: But what could a random old dead dude do!, how could he help us?");

        strcpy(op1[2].scene,"Thoth: You will know when the time is right...");
        strcpy(op1[2].option1Text,"Option1: OK, and how do I enter this 'Underworld' uh world");
        strcpy(op1[2].option2Text,"Option2: Whatever old man, just tell me how to enter the underworld?");

        strcpy(op1[3].scene,"Thoth: On the first new day, when the belt of orion alligns with the horizon the gates of the underworld will open? head eastwords from here and you will see them.\nMake haste for the destined night is merely two days away!");
        strcpy(op1[3].option1Text,"Option1: Poet are we?");
        strcpy(op1[3].option2Text,"Option2: Ok bye old man.");

        strcpy(op1[4].scene,"Thoth: Hmphhh\nWhen the fated night arrives, surely enough you see some entrance emerging in the ground, however there are two of them!");
        strcpy(op1[4].option1Text,"Option1: Go to the larger entrance point");
        strcpy(op1[4].option2Text,"Option2: Go to the smaller entrance point");

        strcpy(op1[5].scene,"As you descend down the norrow hole,it keeps getting narrower, soon it starts feeling heavier inside and your mana leavels start plumitting");
        strcpy(op1[5].option1Text,"Option1: Head back to the surface");
        strcpy(op1[5].option2Text,"Option2: continueue ahead!");

        strcpy(op1[6].scene,"As you move ahead the its difficult to move, the heat is extreme, and your mana decreases\nA question appears \n'I walk on four legs in the morning, two legs in the noon and three legs in the evening'");
        strcpy(op1[6].option1Text,"Option1: A Human");
        strcpy(op1[6].option2Text,"Option2: A centaur");

        strcpy(op1[7].scene,"The path expends and you're teleported to the underworld\nIt's a wast desert where the souls of the damned are tortured for an eternity, and in front of you, you see the giant stature of Amet, the pet of Anubis.\n Amet: who are you traveler and why have you entered the domain of the dead when you yourself aren't dead yet?");
        strcpy(op1[7].option1Text,"Option1: I came here on following the instructions of Thoth");
        strcpy(op1[7].option2Text,"Option2: Thoth sent me here");

        strcpy(op1[8].scene,"Amet: that old man should really be minding his own business, anyhow I can't let you in, I have strict orders from Anubis");
        strcpy(op1[8].option1Text,"Option1: What if I showed you this(The eye of Horus)");
        strcpy(op1[8].option2Text,"Option2: My eye of Horus says otherwise!");

        strcpy(op1[9].scene,"Amet: See, kid that thing has no value here, but since you're brave enough to come here, how about we make a deal, I eat half of you're mana and let you past me?");
        strcpy(op1[9].option1Text,"Option1: sure, suit yourself!");
        strcpy(op1[9].option2Text,"Option2: How about I break you jaw instead!");

        strcpy(op1[10].scene,"You manage to defeat Amet and reach the chamber of Anubis, where you finally meet him\nAnubis: Oh! You finally made it here Horus");
        strcpy(op1[10].option1Text,"Option1: Wait who? I'm not horus...");
        strcpy(op1[10].option2Text,"Option2: You're right, wait what... no,no there's been a misunderstanding, I'm no Horus");

        strcpy(op1[11].scene,"Anubis: Oh! but your aura feels exactly like Horus's, so tell me why are you here? what's the purpose of your visit");
        strcpy(op1[11].option1Text,"Option1: I came here to take Imhotep back to the world of living");
        strcpy(op1[11].option2Text,"Option2: Nothing much, just give up Imhotep");

        strcpy(op1[12].scene,"Anubis: I would do that however he's not in my costudy anymore, I'm done judging him and he's serving his sentence under Osyrus");
        strcpy(op1[12].option1Text,"Option1: So where can I find Osyrus");
        strcpy(op1[12].option2Text,"Option2: Take me to Osyrus");

        strcpy(op1[13].scene,"As you descend down the large hole,it keeps getting larger, soon it starts feeling like vaccum inside and your mana leavels start plumitting");
        strcpy(op1[13].option1Text,"Option1: Head back to the surface");
        strcpy(op1[13].option2Text,"Option2: continueue ahead!");

        strcpy(op1[14].scene,"As you move ahead the its difficult to move, the heat is extreme, and your mana decreases\nA question appears \n'Which god added five extra days to the Calander?'");
        strcpy(op1[14].option1Text,"Option1: Thoth");
        strcpy(op1[14].option2Text,"Option2: Amun Ra");

        strcpy(op1[15].scene,"After Amet consumes half of tour mana, she lets you go past her.\nAnubis: Oh! You finally made it here Horus");
        strcpy(op1[15].option1Text,"Option1: Wait who? I'm not horus...");
        strcpy(op1[15].option2Text,"Option2: You're right, wait what... no,no there's been a misunderstanding, I'm no Horus");

        int i = 0;          //variable to store the scene number
        int k = 0;          //variable to store the choice
        int large,small,large_wrong,small_wrong;   
        large = 0;
        small = 0;
        large_wrong = 0;
        small_wrong = 0;

        while(i<=15){           //loop to display the scenes
            printf("%s\n",op1[i].scene);
            setCyanColor();
            printf("%s\n",op1[i].option1Text);
            resetColor();
            printf("%s\n",op1[i].option2Text);
            setMagentaColor();
            scanf("%d",&k);
            resetColor();

            if (i == 12){
                i = 16;
            }

            if (i == 4 && k == 1){
                large = 1;
                i = 12;
            }
            if (i == 4 && k == 2){
                small = 1;
            }
            if (i == 14 && large){
                i = 6;
            }
            if (i == 5 && k == 1){
                small = 0;
                i = 3;
            }
            if (i == 13 && k == 1){
                large = 0;
                i = 3;
            }
            if (i == 6 && k == 2){
                printf("\nWrong answer");
                i = 4;
            }
            if (i == 14 && k == 2){
                printf("\nWrong answer");
                i = 4;
            }
            if (i == 9 && k == 2){

                i = 14;
            }
            if (i == 15 ){
                i = 9;
            }
            

            i++;

        }

        
    }


    void post_underworld() {
        Scene op1[50];                  //array of structures to store the scenes
        strcpy(op1[0].scene,"Anubis summons a portal and through whic you can go ahead and meet Osyrus\nSet: What? it can't be what are you doing here Horus, you should be in your prision.");
        strcpy(op1[0].option1Text,"Option1: Jail?");
        strcpy(op1[0].option2Text,"Option2: HUH!");

        strcpy(op1[1].scene,"Set: Wait! you're not Horus... who are you and what are you doing here?");
        strcpy(op1[1].option1Text,"Option1: Well I came here to meet Osyrus, who are you?");
        strcpy(op1[1].option2Text,"Option2: who the hell are you?");

        strcpy(op1[2].scene,"Set: Well I am the rightful RA I am Set!");
        strcpy(op1[2].option1Text,"Option1: Oh so you are the one causing all this mischief? get ready to fight me.");
        strcpy(op1[2].option2Text,"Option2: Forgive me my lord for I failed to recognize you!");

        strcpy(op1[3].scene,"Set: Huh! good, I like you");
        strcpy(op1[3].option1Text,"Option1: my liege, what can i do for you?");
        strcpy(op1[3].option2Text,"Option2: How can I assist your glorious purpose my lord?");

        strcpy(op1[4].scene,"Set: First of all carry the body of Osyrus for me.");
        strcpy(op1[4].option1Text,"Option1: Yes sir!");
        strcpy(op1[4].option2Text,"Option2: Gladly!");

        strcpy(op1[5].scene,"As you carry dead osyrus with you, you can see the eye of hours starts glowing violent red!");
        strcpy(op1[5].option1Text,"Option1: But sir? were you not stripped of your powers? how can you appear here in your full glory?");
        strcpy(op1[5].option2Text,"Option2: But sir? how come you were able to kill Osyrus?");

        strcpy(op1[6].scene,"Set: Why do you ask? Do you dought my abilities? but you don't feel like a threat so I suppose Ican tell you? Iam Imhotep! and Iam in symbiotic relationship with Set, we are one");
        strcpy(op1[6].option1Text,"Option1: 0o0");
        strcpy(op1[6].option2Text,"Option2: 0 o 0");

        strcpy(op1[7].scene,"Once I take over the body of a god I will become way stronger than Set alone ever was ");
        strcpy(op1[7].option1Text,"Option1: Greater than even Set?");
        strcpy(op1[7].option2Text,"Option2: Oh that's great ");

        strcpy(op1[8].scene,"After set takes over Osyrus's body he then turns on you...\nSet: now give me those powers of Horus, Die\nYou: So you were just playing along , becouse you were planninng to kill me from the moment we met...");
        strcpy(op1[8].option1Text,"Option1: Fight!");
        strcpy(op1[8].option2Text,"Option2: Fight!");

        strcpy(op1[9].scene,"Game over! you lost");
        strcpy(op1[9].option1Text," ");
        strcpy(op1[9].option2Text," ");

        strcpy(op1[10].scene,"You win!");
        strcpy(op1[10].option1Text," ");
        strcpy(op1[10].option2Text," ");

        int i, fight_early,k;               //variables to store the scene number and the choice
        fight_early = 0;                    //variable to store if the fight happened early
        i = 0;                              //initialize the scene number
        if_kills:                           //label to go back to the start of the game if lost
        while(i < 11){

            printf("%s\n",op1[i].scene);
            setCyanColor();
            printf("%s\n",op1[i].option1Text);
            resetColor();
            setMagentaColor();
            printf("%s\n",op1[i].option2Text);
            resetColor();
            scanf("%d",&k);

            if ( i == 2 && k ==1){
                fight_early = 1;
                battle_mechanincs();
                if (won == true){
                    setGreenColor();
                    printf("You win!!!\n");
                    resetColor();
                    break;
                }
                else{
                    setRedColor();
                    printf("Game over! you lost\n");
                    resetColor();
                    break;
                }
                i = 8;

            }

            if ( i == 8) {
                battle_mechanincs();
                if (won == true){
                    setGreenColor();
        printf("You win!!!\n");
        resetColor();
        break;
    }
    else{
        setRedColor();
        printf("Game over! you lost\n");
        resetColor();
        break;
    }
            }
            i++;
        }
    if(i==9){
        printf("Do you want to try again? 1 for yes 0 for no");
        scanf("%d",&k);
        if(k==1){
        i=0;
        goto if_kills;}
        else{
            
            printf("Game over! you lost\n");
        }
    }

    
    }
    int main()
{
    char text0[] = "CCCCCCCCCCC     SSSSSSSSSS         1             0000           22222222222222\n"; // Text to be animated
    char text1[] = "C               S                1 1          0        0                    2\n";
    char text2[] = "C               S               1  1          0        0                   2\n";
    char text3[] = "C               S                  1          0        0                 2\n";
    char text4[] = "C               SSSSSSSSSS         1          0        0               2\n";
    char text5[] = "C                        S         1          0        0              2\n";
    char text6[] = "C                        S         1          0        0            2\n";
    char text7[] = "C                        S         1           0      0           2\n";
    char text8[] = "CCCCCCCCCCC     SSSSSSSSSS      1111111          0000           222222222222222\n\n\n\n\n\n";
    char text9[] = "Rahul Jilowa -- 2301AI18\n";
    char text10[] = "Juhi Sahni -- 2301CB18\n";
    char text11[] = "Vanshika Agrawal -- 2301CB45\n";
    char text12[] = "Tejaswini Pasuparthi -- 2301CT18\n";
    char text13[] = "Amritansh Rai -- 2301CE33\n";
    char text14[] = "Gautam Kumar -- 2301CE37\n";
    char text15[] = "Harshit -- 2301CS18\n";
    char text16[] = "Sameeksha Thammalla -- 2301CS46\n";
    char text17[] = "Navyaa Mittal -- 2301EE18\n";
    char text18[] = "Nishtha Mahesh Pandey -- 2301EC18\n";

    char *texts[] = {text0, text1, text2, text3, text4, text5, text6, text7, text8,text9,text10,text11,text12,text13,text14,text15,text16,text17,text18};
    int numLines = sizeof(texts) / sizeof(texts[0]);
    int textLengths[] = {sizeof(text0) - 1, sizeof(text1) - 1, sizeof(text2) - 1, sizeof(text3) - 1, sizeof(text4) - 1, sizeof(text5) - 1, sizeof(text6) - 1, sizeof(text7) - 1, sizeof(text8) - 1,sizeof(text9) - 1, sizeof(text10) - 1, sizeof(text11) - 1, sizeof(text12) - 1, sizeof(text13) - 1, sizeof(text14) - 1, sizeof(text15) - 1, sizeof(text16) - 1, sizeof(text17) - 1, sizeof(text18) - 1};

    for (int i = 0; i < numLines; ++i) {
        // Clear the screen
        if (i == 0){
            printf("\033[H\033[J");
        }
        

        // Print the text up to the current character
        for (int j = 0; j < textLengths[i]; ++j) {
            printf("%c", texts[i][j]);
            fflush(stdout);
            usleep(10000); // 10,000 microseconds = 0.01 second
        }

        // Wait for a short duration at the end of each line
        usleep(300000); // 300,000 microseconds = 0.3 second
    }


 setBlueColor();
 printf("Welcome to the start of the game\n");          
 resetColor();
 earth_city();
 wisdom_city();
 printf("which city do you want to go next?\n");
 printf("1. Water city\n2. Wind city\n");
 int choice_city;
 scanf("%d",&choice_city);
 if (choice_city == 1){
     water_city();
     printf("Now you are going to the wind city\n");
     wind_city();
 }
 else if (choice_city == 2){
     wind_city();
     printf("Now you are going to the water city\n");
     water_city();
 }
 else{
     printf("Invalid choice");
 }
  underworld();
  post_underworld();
    return 0;
}
