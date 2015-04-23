#ifndef DEATHFLAG_H_INCLUDED
#define DEATHFLAG_H_INCLUDED


bool  death(Player p){
  bool dead;
  if(p.HP ==0){
    dead = true;
  }
  else{
    dead =false;
  }
  return dead;
}
#endif
