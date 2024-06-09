#include <iostream>
#include <string>
#include <typeinfo>

class Encryption
{
private:
	long int key_for_encrypt;
	long int key_for_decrypt;
	long int encrypted_input;
	long int decrypted_input;
	bool* key_for_encrypt_mass;
	bool* key_for_decrypt_mass;
	bool* encrypted_input_mass;
	bool* decrypted_input_mass;
protected:

public:
	Encryption()
	{
		key_for_encrypt = 0;
		key_for_decrypt = 0;
	}
	long int encrypt(long int encrypted_input_, long int key_for_encrypt_)
	{
		encrypted_input = encrypted_input_;
	}
};
class DES
{
private:
	long int key_a=0;
	long int input_a=0;
	bool* key_mass;
	bool* input_mass;

	
public:
	bool* FromTenToTwo(int ten, int num_of_bits)
	{
		int i = 0;
		bool* buffer;
		bool* output;
		buffer = (bool*)calloc(num_of_bits, sizeof(bool));
		output = (bool*)calloc(num_of_bits, sizeof(bool));
		while (ten != 0)
		{

			buffer[i] = ten % 2;
			ten = ten / 2;
			i++;
		}
		for (int j = 0; j < num_of_bits; j++)
		{
			output[num_of_bits - 1 - j] = buffer[j];
		}
		return output;
	}
	DES(long int key_, long int input_)
	{
		key_a = key_;
		input_a = input_;
		key_mass = (bool*)calloc(64, sizeof(bool));
		input_mass = (bool*)calloc(64, sizeof(bool));
	}
	bool* DeleteHexBitsFromKey(bool* key_64)
	{
		bool* key_56 = (bool*)calloc(56, sizeof(bool));
		int j = 0;
		for(int i = 0; i < 64; i++)
		{
			
			if (i % 8 != 7)
			{
				key_56[i - j] = key_64[i];
			}
			else 
			{
				j++;
			}
		}
		return key_56;
	}
	bool* AddHexBitsFromKey(bool* key_56)
	{
		bool* key_64 = (bool*)calloc(64, sizeof(bool));
		int j = 0;
		for (int i = 0; i < 56; i++)
		{

			if (i % 8 != 7)
			{
				key_64[i + j] = key_56[i];
			}
			else
			{
				j++;
				int num_one = 0; // Counter of '1' in previous byte
				for (int k = 1; k < 8; k++)
				{
					if (key_64[i - k] == 1)
					{
						num_one++;
					}
				}
				if (num_one % 2 == 0) 
				{
					key_64[i] = 1;
				}
				else
				{
					key_64[i] = 0;
				}
			}
		}
		return key_64;
	}
	bool* S_Block(int* table, bool* input, int size)
	{
		bool* buffer = new bool[size];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = input[table[i]];
		}
		return buffer;
	}
	bool* S_Block_64_56(bool* input)
	{
		int table[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3 ,60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };
		bool* buffer = new bool[56];
		for (int i = 0; i < 56; i++)
		{
			buffer[i] = input[table[i]-1];
		}
		return buffer;
	}
	bool* CiclicLeftShift(bool* input, int size, int num) // num - величина сдвига
	{
		num = num % size;
		bool* buffer = new bool[size];
		for (int i = 0; i < size; i++)
		{
			buffer[i] = input[(i + num)%size];
		}
		
		return buffer;
	}
	bool* S_Block_56_48(bool* input)
	{
		int table[48] = {14, 17, 11, 24, 1,	5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };
		bool* buffer = new bool[48];
		for (int i = 0; i < 48; i++)
		{
			buffer[i] = input[table[i] - 1];
		}
		return buffer;
	}
	bool** RoundKeysGeneration(bool* key_56)
	{
		bool* key_added_bits = AddHexBitsFromKey(key_56);
		bool* key_s_block = S_Block_64_56(key_added_bits);

		bool** buffer_CyclShift_C = new bool* [16];
		bool** buffer_CyclShift_D = new bool* [16];
		bool** buffer_CyclShift = new bool* [16];
		for (int i = 0; i < 16; i++)
		{
			buffer_CyclShift[i] = new bool[56];
			buffer_CyclShift_C[i] = new bool[28];
			buffer_CyclShift_D[i] = new bool[28];
			if (i == 0)
			{
				buffer_CyclShift_C[i] = CiclicLeftShift(key_s_block, 28, 1);
				buffer_CyclShift_D[i] = CiclicLeftShift(key_s_block + 28, 28, 1);
			}
			else if (i==1 || i==8 || i==15)
			{
				buffer_CyclShift_C[i] = CiclicLeftShift(buffer_CyclShift_C[i - 1], 28, 1);
				buffer_CyclShift_D[i] = CiclicLeftShift(buffer_CyclShift_D[i - 1] , 28, 1);
			}
			else
			{
				
				buffer_CyclShift_C[i] = CiclicLeftShift(buffer_CyclShift_C[i - 1], 28, 2);
				buffer_CyclShift_D[i] = CiclicLeftShift(buffer_CyclShift_D[i - 1], 28, 2);
			}
			for (int j = 0; j < 28; j++)
			{
				buffer_CyclShift[i][j] = buffer_CyclShift_C[i][j];
				buffer_CyclShift[i][j+28] = buffer_CyclShift_D[i][j];
			}
			buffer_CyclShift[i] = S_Block_56_48(buffer_CyclShift[i]);
		}
		return buffer_CyclShift;
		
	}

	bool* Start_S_Block( bool* input)
	{
		int size = 64;
		int table[64] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };
		return S_Block(table, input, size);
	}
	bool* End_S_Block(bool* input)
	{
		int size = 64;
		int table[64] = { 40,8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55 ,	23, 63, 31,38,6, 46, 14, 54, 22, 62, 30, 37, 5, 45,	13,	53, 21,	61,	29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
34, 2,42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
		return S_Block(table, input, size);
	}

	bool* E_func(bool* input)
	{
		int size = 48;
		int table[48] = { 32,1, 2, 3, 4, 5,4, 5,6,7, 8,	9,8, 9, 10, 11, 12, 13,12, 13, 14, 15, 16, 17,16, 17, 18, 19, 20, 21,20, 21, 22, 23, 24, 25,24, 25, 26, 27, 28, 29,28, 29, 30, 31, 32, 1 };
		return S_Block(table, input, size);
	}
	bool* Feistel_func(bool* input, bool* key_48)
	{
		bool* buffer = E_func(input);
		for (int i = 0; i < 48; i++)
		{
			buffer[i] = buffer[i] ^ key_48[i];
		}
		int S_tables[8][64] = {
			{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 },
			{15,  1,  8, 14,  6, 11,  3,  4,  9 , 7,  2, 13, 12,  0,  5, 10, 3, 13,  4,  7, 15,  2,  8, 14 ,12 , 0 , 1 ,10 , 6 , 9 ,11 , 5, 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
			{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4 , 2 , 8, 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1, 13,  6,  4,  9,  8, 15,  3 , 0 ,11 , 1 , 2 ,12,  5 ,10 ,14 , 7,  1 ,10 ,13 , 0 , 6 , 9 , 8 , 7 , 4 ,15 ,14,  3, 11,  5,  2, 12},
			{7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15, 13,  8 ,11 , 5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9, 10,  6,  9,  0, 12, 11,  7 ,13 ,15 , 1 , 3 ,14 , 5 , 2 , 8 , 4, 3 ,15 , 0 , 6 ,10,  1 ,13,  8 , 9 , 4,  5, 11, 12,  7,  2, 14},
			{2, 12,  4,  1 , 7 ,10 ,11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3 , 0 ,14,11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
			{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3 , 4, 14,  7,  5, 11, 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,  9, 14, 15,  5,  2,  8, 12,  3 , 7 , 0 , 4 ,10 , 1 ,13, 11,  6, 4 , 3 , 2 ,12,  9,  5, 15, 10, 11, 14,  1,  7,  6 , 0 , 8, 13},
			{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1, 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8 , 0 , 5 , 9 , 2,  6, 11, 13,  8,  1,  4 ,10 , 7 , 9 , 5 , 0 ,15, 14 , 2 , 3 ,12},
			{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7, 1, 15, 13,  8, 10,  3,  7,  4 ,12 , 5 , 6 ,11 , 0 ,14 , 9, 2,7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
		};
		bool* s_bl_buffer = new bool[48];
		for (int i=0; i<8;i++)
		{
			int str_num = buffer[i + 1] * 2 + buffer[i + 6];
			int stlb_num = buffer[i + 2] * 8 + buffer[i + 3]*4 + buffer[i+4]*2 + buffer[i+5];
			int t_num = S_tables[i][str_num * 16 + stlb_num];
			bool* num_bit_4 = FromTenToTwo(t_num, 4);
			for (int j = 0; j < 4; j++)
			{
				s_bl_buffer[i + j] = num_bit_4[j];
			}
		}

		int table_P[32] = { 16,  7,  20,  21,29,  12,  28,  17,1,  15,  23,  26,5,  18,  31,  10,2,  8,  24,  14,32,  27,  03,  9,19,  13,  30,  6,22,  11,  04,  25 };
		
		return S_Block(table_P, s_bl_buffer, 32);
	
	
	
	
	
	}

	bool* Feistel_Network(bool* input, bool** keys,  int rounds_num)
	{
		bool output[64];
		bool L[32];
		bool R[32];
		bool L_buf[32];
		bool R_buf[32];
		for (int i = 0; i < 32; i++)
		{
			L[i] = input[i];
			R[i] = input[32 + i];
		}
		for (int i = 0; i < rounds_num; i++)
		{
			bool* func_res = Feistel_func(R, keys[i]);
			for (int j = 0; j < 32; j++)
			{
				L_buf[j] = R[j];
				R_buf[j] = L[j] ^ func_res[j];
				L[j] = L_buf[j];
				R[j] = R_buf[j];
			}

		}
		for (int i = 0; i < 32; i++)
		{
			
			output[i] = L[i];
			output[i + 32] = R[i];
		}
		return output;
	}


	bool* encrypt(bool* input, bool* key)
	{
		bool** round_keys = RoundKeysGeneration(key);
		bool* buffer;
		buffer = Start_S_Block(input);
		buffer = Feistel_Network(buffer, round_keys,16);
		buffer = End_S_Block(buffer);
		return buffer;
	}

};
int main()
{
	DES a(1, 1);
	bool* c = a.FromTenToTwo(254, 64);
	bool* b;
	b = a.DeleteHexBitsFromKey(c);
	std::cout << std::endl;
	for (int i = 0; i < 56; i++)
	{
		std::cout << b[i] << " ";
	}
	std::cout << std::endl;
	bool* d = a.CiclicLeftShift(b, 56, 132);
	for (int i = 0; i < 56; i++)
	{
		std::cout << d[i] << " ";
	}
}