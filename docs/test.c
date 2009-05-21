void LinearConvolution(double X[],double Y[], double Z[], int lenx, int leny)
{
	double *zptr,s,*xp,*yp;
	int lenz;
	int i,n,n_lo,n_hi;

	lenz=lenx+leny-1;
	zptr=Z;

	for (i=0;i<lenz;i++) {
		s=0.0;
		n_lo=0>(i-leny+1)?0:i-leny+1;
		n_hi=lenx-1<i?lenx-1:i;
		xp=X+n_lo;
		yp=Y+i-n_lo;
		for (n=n_lo;n<=n_hi;n++) {
			s+=*xp * *yp;
			xp++;
			yp--;
			}
		*zptr=s;
		zptr++;
		}
}
