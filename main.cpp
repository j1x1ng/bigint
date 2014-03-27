/*
 *built by @jx(@j1x1ng).
 *support : + - * / (mod)
 *with at most 1K length bigint.
 *the bigint is saved in a digits-arrays(int type)
 *it gives you many degrees to use it.
 */

#include <iostream>
#include <string>
using namespace std;
const int siz = 100;
const int err = -2;
int A[siz], B[siz], C[siz], D[siz], M[siz];
int ER[4*siz][2*siz];
bool flag_of_er_init = false;
///0 : length
///1...a[0] : signed-digits

int bigint_comp(int *a, int *b);
int bigint_comp(int *a, int b);
inline void bigint_opp(int *d);
inline void bigint_shiftleft(int *d);
inline void bigint_shiftright(int *d);
inline void bigint_fromint(int x, int *d);
inline void bigint_fromstr(string buf, int *d);
inline void bigint_in(istream &in, int *d);
inline void bigint_out(ostream &out, int *d);
inline void bigint_eout(ostream &out, int *d); // for debug.
inline void bigint_adjust(int *c);
inline void bigint_plus(int *a, int *b, int *c);
inline void bigint_times(int *a, int *b, int *c);
inline void bigint_divide(int *a, int *b, int *c, int *g);
inline void bigint_pow(int *a, int *b, int *c, int *r); // a^b mod c
inline void bigint_ER_init();
inline void bigint_minus(int *a, int *b, int *c);
inline void bigint_cpy(int *a, int *b);
inline void bigint_inc(int *a);
inline void bigint_dec(int *a);
inline void bigint_divide_adjust(int *b, int *c, int *g, int *e);
///only use in bigint_divide(), dont refer to this func in other scope.

bool bigint_isprime(int *a);

int main(){
    ///a simple example including : (signed) a+b a*b a/b a^b (mod M)

    while(1){
      cout << "input A, B, M (departed by spaces) :";

      bigint_in(cin, A);
      bigint_in(cin, B);
      bigint_in(cin, M);

      cout << "A+B = ";
      bigint_plus(A, B, C);
      bigint_out(cout, C);
      cout << endl;

      cout << "AxB = ";
      bigint_times(A, B, C);
      bigint_out(cout, C);
      cout << endl;

      cout << "A/B = ";
      bigint_divide(A, B, C, D);
      bigint_out(cout, C);
      cout << " ... ";
      bigint_out(cout, D);
      cout << endl;

      bigint_pow(A, B, M, C);
      cout << "A^B = ";
      bigint_out(cout, C);
      cout << " ( mod M ) " << endl;
    }
    return 0;
}

bool bigint_isprime(int *a){

    return false;
}

int bigint_comp(int *a, int b){
    int c[siz];
    bigint_fromint(b, c);
    return bigint_comp(a, c);
}

inline void bigint_inc(int *a){
    if(a[0] == err)
      return ;
    int lim = 9;
    if(a[ a[0] ] < 0)
      lim = 0;
    ++ a[1];
    a[ ++a[0] ] = 0;
    for(int i = 1; i <= a[0]; ++i)
      if(a[i] > lim){
        a[i] -= 10;
        ++ a[i+1];
      }
    while(a[0]>1 && a[ a[0] ]==0)
      -- a[0];
    return ;
}

inline void bigint_dec(int *a){
    if(a[0] == err)
      return ;
    int lim = 0;
    if(a[ a[0] ] < 0)
      lim = -9;
    -- a[1];
    a[ ++a[0] ] = 0;
    for(int i = 1; i <= a[0]; ++i)
      if(a[i] < lim){
        a[i] += 10;
        -- a[i+1];
      }
    while(a[0]>1 && a[ a[0] ]==0)
      -- a[0];
    return ;
}

inline void bigint_minus(int *a, int *b, int *c){
    bigint_opp(b);
    bigint_plus(a, b, c);
    bigint_opp(b);
    return ;
}

inline void bigint_ER_init(){
    flag_of_er_init = true;
	bigint_fromint(1, ER[0]);
    bigint_fromint(2, ER[1]);
    //2^0 == 1
    int l = 4*siz;
    for(int i = 2; i < l; ++i){

      bigint_fromint(0, ER[i]);
      bigint_times(ER[i-1],ER[1], ER[i]);
     // cerr<<i;bigint_out(cerr,ER[i]);cerr<<endl;
    }
    return ;
}

inline void bigint_pow(int *a, int *b, int *c, int *r){

    ///a^b mod c a>0 && b>0 && c!=0

    if(a[0]==err || b[0]==err || a[ a[0] ]<0 || b[ b[0] ]<0 || (c[1]==0&&c[0]==1)){
      r[0] = err;
      return ;
    }
    if(!flag_of_er_init)
      bigint_ER_init();

    int l = b[0]*4, CC=0;
    int d[2][siz], e[siz];
    bigint_fromint(1, d[0]);

    for(int i = l; i >= 0; --i){
      bigint_times(d[CC], d[CC],e);
      bigint_divide(e, c, d[1-CC], d[CC]);
      if(bigint_comp(ER[i], b) <= 0){
        bigint_fromint(0, e);
        bigint_minus(b, ER[i], e);
        bigint_cpy(e, b);
        bigint_fromint(0, e);
        bigint_times(d[CC], a, e);
        bigint_divide(e, c, d[CC], d[1-CC]);
        CC = 1 - CC;
      }
      //bigint_out(cerr, d[CC]);cerr << endl;
    }
    bigint_cpy(d[CC], r);
    return ;
}

inline void bigint_cpy(int *a, int *b){
    for(int i = 0; i <= a[0]; ++i)
      b[i] = a[i];
    return ;
}

inline void bigint_divide_adjust(int *b, int *c, int *g, int *e){
    if(bigint_comp(g, 0) != 0){
      bigint_inc(c);
      bigint_minus(b, g, e);
      bigint_cpy(e, g);
    }
    return ;
}

inline void bigint_divide(int *a, int *b, int *c, int *g){
    if(a[0]==err || b[0]==err){
      c[0] = g[0] = err;
      return ;
    }
    if(b[0]==1 && b[1]==0){
      c[0] = g[0] = err;
      return ;
    }
    if(a[0]==1 && a[1]==0){
      bigint_fromint(0, c);
      bigint_fromint(0, g);
      return ;
    }
    int d[siz], e[siz], f[siz], k;
    bool flag = false;
    bigint_fromint(0, e);
    bigint_fromint(0, d);

    if(a[ a[0] ]<0 && b[ b[0] ]<0){
      bigint_opp(a);
      bigint_opp(b);
      bigint_divide(a, b, c, g);
      bigint_divide_adjust(b, c, g, e);
      bigint_opp(a);
      bigint_opp(b);
      return ;
    }

    if(a[ a[0] ]<0 && b[ b[0] ]>0){
      bigint_opp(a);
      bigint_divide(a, b, c, g);
      bigint_divide_adjust(b, c, g, e);
      bigint_opp(c);
      bigint_opp(a);
      return ;
    }

    if(a[ a[0] ]>0 && b[ b[0] ]<0){
      bigint_opp(b);
      bigint_divide(a, b, c, g);
      bigint_opp(c);
      bigint_opp(b);
      return ;
    }

    if(bigint_comp(a, b) < 0){
      c[0] = 1;
      c[1] = 0;
      bigint_cpy(a, g);
      return ;
    }
    bigint_fromint(0, c);
    c[0] = a[0];
    for(int i = a[0]; i >= 1; --i){
      d[1] = a[i];
      bigint_fromint(0, e);
      for(k = 1; k <= 10; ++k){
        bigint_fromint(k, f);
        bigint_times(b, f, e);
        if(bigint_comp(d, e) < 0)
          break;
      }
      if(k == 11)
        continue;
      c[i] = --k;
      // try shang = k
      bigint_fromint(k, f);
      // k*b(chu shu) ==> e
      bigint_times(f, b, e);
      // e ==> -e
      bigint_opp(e);
      // f' <== d - e
      bigint_plus(d, e, f);
      bigint_cpy(f, d);
      bigint_shiftleft(d);
      ///bigint_adjust(d);
    }
    bigint_adjust(c);
    bigint_shiftright(d);
    bigint_cpy(d, g);
    return ;
}

inline void bigint_opp(int *d){
    for(int i = 1; i <= d[0]; ++i)
      d[i] = -d[i];
    return ;
}

int bigint_comp(int *a, int *b){
    if(a[0]==err || b[0]==err)
      return err;
    if(a[0] < b[0])
      return -1;
    else if(a[0] > b[0])
      return 1;
    for(int i = a[0]; i >= 1; --i)
      if(a[i] < b[i])
        return -1;
      else if(a[i] > b[i])
        return 1;
    ///equal.
    return 0;
}

inline void bigint_shiftleft(int *d){
    // d *= 10;
    if(d[0] == err)
      return ;
    if(d[0]==1 && d[1]==0)
      return ;
    ++ d[0];
    for(int i = d[0]; i > 1; --i)
      d[i] = d[i-1];
    d[1] = 0;
    return ;
}

inline void bigint_shiftright(int *d){
    // d /= 10;
    if(d[0] == err)
      return ;
    -- d[0];
    for(int i = 1; i <= d[0]; ++i)
      d[i] = d[i+1];
    if(d[0] == 0){
      d[0] = 1;
      d[1] = 0;
    }
    return ;
}

inline void bigint_fromint(int x, int *d){
    int p = 0;
    for(int i = 0; i < siz; ++i)
      d[i] = 0;
    if(x == 0){
      d[0] = 1;
      return ;
    }

    if(x > 0){
      while(x > 0){
        ++ p;
        d[p] = x%10;
        x /= 10;
      }
    }
    else{
      x = -x;
      while(x > 0){
        ++ p;
        d[p] = x%10;
        x /= 10;
      }
    }
    d[0] = p;
    return ;
}

inline void bigint_fromstr(string buf, int *d){
    int l, x;
    l = buf.length();
    for(int i = 0; i < l; ++i)
      if(buf[i]<'0' || buf[i]>'9'){
        if(i==0 && buf[i]=='-')
          continue;
        d[0] = err;
        return ;
      }
    if(buf[0] == '-'){
      d[0] = l-1;
      for(int i = 1; i <= d[0]; ++i)
        d[l-i] = -(int)(buf[i]-'0');
    }
    else{
      d[0] = l;
      for(int i = 0; i < l; ++i)
        d[l-i] = (int)(buf[i]-'0');
    }
    while(d[0]>1 && d[ d[0] ]==0)
      -- d[0];
    return ;
}

inline void bigint_in(istream &in, int *d){
    bigint_fromint(0, d);
    string buf;
    in >> buf;
    bigint_fromstr(buf, d);
    return ;
}

inline void bigint_out(ostream &out, int *d){
    if(d[0] == err){
      out << "#WRONG_FORM#";
      return ;
    }
    if(d[0] == 0){
      out << 0;
      d[0] = 1;
      d[1] = 0;
      return ;
    }

    if(d[ d[0] ] < 0){
      out << '-';
      for(int i = d[0]; i >= 1; --i)
        out << -d[i];
    }
    else for(int i = d[0]; i >= 1; --i)
        out << d[i];
    return ;
}

inline void bigint_adjust(int *c){
    while(c[0]>1 && c[ c[0] ]==0)
      -- c[0];
    if(c[ c[0] ] > 0){
      for(int i = 1; i < c[0]; ++i)
        while(c[i] < 0){
          -- c[i+1];
          c[i] += 10;
        }
    }
    else{
      for(int i = 1; i < c[0]; ++i)
        while(c[i] > 0){
          ++ c[i+1];
          c[i] -= 10;
        }
    }
    ++ c[0];
    while(c[0]>1 && c[ c[0] ]==0)
      -- c[0];
    return ;
}

inline void bigint_plus(int *a, int *b, int *c){
    if(a[0]==err || b[0]==err){
      c[0] = err;
      return ;
    }
    int l = a[0];
    if(b[0] > l)
      l = b[0];
    c[0] = l+1;
    for(int i = 1; i <= c[0]; ++i)
      c[i] = 0;

    for(int i = 1; i <= l; ++i){
      c[i] += a[i]+b[i];
      while(c[i] > 9){
        ++ c[i+1];
        c[i] -= 10;
      }
      while(c[i] < -9){
        -- c[i+1];
        c[i] += 10;
      }
    }
    bigint_adjust(c);
    return ;
}

inline void bigint_eout(ostream &out, int *d){
    for(int i = d[0]; i >= 1; --i)
      out << ' ' << d[i];
    return ;
}

inline void bigint_times(int *a, int *b, int *c){
    c[0] = a[0]+b[0]+1;
    for(int i = 1; i <= c[0]; ++i)
      c[i] = 0;
    for(int i = 1; i <= a[0]; ++i)
      for(int j = 1; j <= b[0]; ++j){
        c[i+j-1] += a[i]*b[j];
        while(c[i+j-1] > 9){
          ++ c[i+j];
          c[i+j-1] -= 10;
        }
        while(c[i+j-1] < -9){
          -- c[i+j];
          c[i+j-1] += 10;
        }
      }
    bigint_adjust(c);
    return ;
}
