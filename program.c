int main() {
	int a = 1;
	int _b;
	int c, d, e;
	int u = -1;
	int f = +1;
	int cst = 124;
	int teste = (a == _b ? 5 : 10);
	float g = 0;

	f++;
	f--;
	f += 1;
	f -= 1;
	f /= 1;
	f *= 1;
	f %= 1;
	f = a & b;
	f = a | b;
	f = a ^ b;
	f = ~a;
	f = a << 2;
	f = a >> 2;

	cst = 123.45;
	cst = .11;
	cst = 11.;

	while(a != 0) {
		continue;
	}

	do {
	} while(a > 0);

	for (int i = 0; i < 10; ++i) {
		break;
	}

	for (; ; ) {
		break;
	}

	if (a == e && b != c) {
	} else if (a == 0 || a <= 0 || a >= 0) {
	} else {
	}

	switch(a) {
		case 1:
		break;
		default:
		break;
	}

	main();
	return 1;
}