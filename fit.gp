#!/usr/bin/gnuplot -persist

f="log/info_1/log000.dat"
f1="log/log000.dat"
f2="log/log060.dat"
f3="log/log090.dat"
fs="log/log090_s.dat"
f=f


tau_min=100
tau_max=1000
t_min=0.32
A=2.7387531258849548976		# true value for A
T=0.5
C=0.0589547
C1=-2.5
C2=2.1
L1=0.51762191809152735544	# true value for L1
L2=1.64965			# true value for L2
B=1e-10
alpha=0.01

FIT_LIMIT=1e-15

t(tau)=T-C*exp(-alpha/A**2*tau)

t1(tau)=T-C*exp(-alpha/A**2*tau+D1*exp(-alpha/A**2*L1*tau))

t11(tau)=T-C*exp(\
-alpha/A**2*tau\
+D1*exp(-alpha/A**2*L1*tau)\
+D2*exp(-2*alpha/A**2*L1*tau)\
)

t2(tau)=T-C*exp(\
-alpha/A**2*tau\
+D1*exp(-alpha/A**2*L1*tau)\
+D2*exp(-2*alpha/A**2*L1*tau)\
+D3*exp(-alpha/A**2*L2*tau)\
+D4*exp(-2*alpha/A**2*L2*tau)\
+D5*exp(-alpha/A**2*(L1+L2)*tau)\
)

g(t)=alpha*(T-t)/(A+C1*(T-t)**L1+C2*(T-t)**L2)**2
g0(t)=alpha*(T-t)/A**2
g1(t)=alpha*(T-t)/(A+C1*(T-t)**L1)**2

logg(t)=log(alpha/A**2)+log(T-t)-2.*(C1*(T-t)**L1-C2*(T-t)**L2)/A

du0(t)=A/sqrt(T-t)
durest(t)=A+C1*(T-t)**L1

D1=1.
D2=0.17
D3=-5.
D4=0.
D5=2.
L2=.1
set logscale y 10
# set terminal postscript
# set output "first_eigenvals.ps"
set xlabel "computational time"
set title "first two stable modes fit\nt2(tau)-t(tau)"

# plot [:5000] abs(log((t2(x)-T)/(t11(x)-T))) t "fit", f u 1:(abs(log(($2-T)/(t11($1)-T)))) w l t "data"

fit [200:] t1(x) f u 1:2 via C,T,A,D1,D2,L1

plot f u 1:(abs(log($2-T)-log(t11($1)-T))) w l t "data"

# fit [500:5000] t2(x) f u 1:2 every 10 via D3,D5,L2

# fit [500:5000]\
# D3*exp(-alpha/A**2*L2*x)\
# +D4*exp(-2*alpha/A**2*L2*x)\
# +D5*exp(-alpha/A**2*(L1+L2)*x)\
# f u 1:(log(($2-T)/(t11($1)-T))) every 10 via D3,D4,D5,L2

# fit [tau_min:tau_max] t1(x) f u 1:2 via C,T,D1
# fit [.32:] du0(x) f u 2:5 via T,A
# B=alpha/A**2


# plot [0:3] logg(x)-log(g(x))
# plot [.32:] durest(x)-A, f u 2:($5*sqrt(T-$2)-A) w l

# fit [.32:] C1*(T-x)**L1 f u 2:($5*sqrt(T-$2)-A) via C1,L1
# fit [.32:] C1*(T-x)**L1 + C2*(T-x)**L2 f u 2:($5*sqrt(T-$2)-A) via C1,L1,C2,L2

# C2=3.0
# fit [.32:] g(x) f u 2:6 via C1,C2,L1,L2


# fit [2000:6000] E1-L1*B*x f u 1:(log(abs(sqrt(C)*exp(-B/2*$1)*$5-A))) via E1,L1
# fit [500:1500] E2-L2*B*x f u 1:(log(abs(sqrt(C)*exp(-B/2*$1)*$5-A+exp(E1-L1*B*$1)))) via E2,L2


# set logscale y 10
#plot  f u 1:(abs(sqrt(C)*exp(-B/2*$1)*$5-A+exp(E1-L1*B*$1))) w l, exp(E2-L2*B*x)
# plot f u 1:(sqrt(C)*exp(-B/2*$1)*$5-A) w l

# set terminal postscript
# set output "first_eigenval.ps"
# set xlabel "computational time"
# set title "first eigenvalue fit"

# plot f u 1:(log(abs(sqrt(C)*exp(-B/2*$1)*$5-A))/log(10)) w l t "log(|sqrt(T-t)*du(0,t)-A|)", (E1-L1*B*x)/log(10) t "linear fit"
# replot E-L*B*x t "linear fit"
