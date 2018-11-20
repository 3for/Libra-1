#include "linear_gkr/verifier.h"
#include <string>
#include <utility>
#include <gmp.h>
#include <gmpxx.h>
#include "linear_gkr/random_generator.h"

void verifier::get_prover(prover *pp)
{
	p = pp;
}

void verifier::read_circuit(const char *path)	
{
	int d;
	static char str[300];
	FILE *circuit_in;
	circuit_in = fopen(path, "r");

	fscanf(circuit_in, "%d", &d);
	int n;
	C.circuit.clear();
	for(int i = 0; i < d; ++i)
	{
		C.circuit.push_back(layer());
		fscanf(circuit_in, "%d", &n);
		int max_gate = -1;
		for(int j = 0; j < n; ++j)
		{
			int ty, g, u, v;
			fscanf(circuit_in, "%d%d%d%d", &ty, &g, &u, &v);
			if(g > max_gate)
				max_gate = g;
			C.circuit[i].gates[g] = std::make_pair(ty, std::make_pair(u, v));
			C.circuit[i].gate_id.push_back(g);
		}
		int cnt = 0;
		while(max_gate)
		{
			cnt++;
			max_gate >>= 1;
		}
		max_gate = 1;
		while(cnt)
		{
			max_gate <<= 1;
			cnt--;
		}
		if(n == 1)
		{
			//add a dummy gate to avoid ill-defined layer.
			C.circuit[i].gate_id.push_back(max_gate);
			C.circuit[i].gates[max_gate] = std::make_pair(2, std::make_pair(0, 0));
		}
		while(max_gate)
		{
			cnt++;
			max_gate >>= 1;
		}
		C.circuit[i].bit_length = cnt;
	}

	fclose(circuit_in);
}

prime_field::field_element add(const layered_circuit &C, int depth, std::vector<prime_field::field_element> z)
{

}
prime_field::field_element mult(const layered_circuit &C, int depth, std::vector<prime_field::field_element> z)
{

}

prime_field::field_element V_0(const std::vector<prime_field::field_element> &r_0, 
								std::vector<std::pair<int, prime_field::field_element> > output)
{
	
	assert(false);
	return prime_field::field_element();
}

gmp_randstate_t rstate;

std::vector<prime_field::field_element> generate_randomness(unsigned int size)
{
	int k = 0;
	while(size)
	{
		k++; size >>= 1;
	}
	std::vector<prime_field::field_element> ret;

	for(int i = 0; i < k; ++i)
	{
		mpz_t random_element;
		mpz_init(random_element);
		mpz_urandomm(random_element, rstate, prime_field::mod.get_mpz_t());
		ret.push_back(mpz_class(random_element));
	}
	return ret;
}

bool verifier::verify()
{
	gmp_randinit_default(rstate);
	p -> proof_init();

	auto result = p -> evaluate();
	printf("evaluation result:\n");
	for(auto x : result)
	{
		printf("%d %s\n", x.first, x.second.to_string(10).c_str());
	}

	prime_field::field_element alpha, beta;
	alpha.value = 1;
	beta.value = 0;
	random_oracle oracle;
	//initial random value
	std::vector<prime_field::field_element> r_0 = generate_randomness(C.circuit[C.circuit.size() - 1].bit_length);
	prime_field::field_element a0 = V_0(r_0, result);

	return false;
}