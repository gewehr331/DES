#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include <functional>

class Encryption
{
private:
	
protected:
	std::vector<bool> buffer_ops;
	std::vector<bool>& FromTenToTwo(long long ten, int size)
	{
		buffer_ops.erase(buffer_ops.begin(), buffer_ops.end());
		std::vector<bool> buffer;

		while (ten != 0)
		{
			buffer.push_back(ten % 2);
			ten = ten / 2;
		}
		buffer_ops.resize(size,0);
		
		for (unsigned int j = 0; j < buffer.size(); j++)
		{
			buffer_ops[size - 1 - j] = buffer[j];
		}
		for (unsigned int j = 0; j < size - buffer.size(); j++)
		{
			buffer_ops[j] = 0;
		}
		return buffer_ops;
	}
public:
	Encryption()
	{

	}
	long int encrypt(long int decrypted_input_, long int key_for_encrypt_)
	{
		//encrypt(FromTenToTwo(decrypted_input_), FromTenToTwo(key_for_encrypt_));
	}
	long int decrypt(long int encrypted_input_, long int key_for_decrypt_)
	{
		//decrypt(FromTenToTwo(encrypted_input_), FromTenToTwo(key_for_decrypt_));
	}
	std::vector<bool> &encrypt(std::vector<bool> &decrypted_input_, std::vector<bool> &key_for_encrypt_)
	{

	}
	std::vector<bool> &decrypt(std::vector<bool> &encrypted_input_, std::vector<bool> &key_for_decrypt_)
	{

	}
};

class FeistelNetwork: public Encryption
{
private:
	 std::vector<bool> xor_results;

	 std::vector<bool> &Xor(std::vector<bool> &a, std::vector<bool> &b)
	{
		xor_results.erase(xor_results.begin(), xor_results.end());
		xor_results.resize(a.size(), 0);
		for (unsigned int i = 0; i < a.size(); i++)
		{
			xor_results[i] = a[i] ^ b[i];
		}
		return xor_results;
		
	}
	std::vector<bool> Feistel_Network_buf;
	std::vector<bool> error;
protected:
	std::vector<bool> &(*FeistelFunction)(std::vector<bool> &a, std::vector<bool>&b);
	std::vector<bool> &Feistel_Network(std::vector<bool> &input, std::vector<std::vector<bool>> &keys, int rounds_num)
	{
		Feistel_Network_buf.erase(Feistel_Network_buf.begin(), Feistel_Network_buf.end());
		
		Feistel_Network_buf.resize(input.size()*2,0);
		std::vector<bool> L;
		std::vector<bool> R;
		std::vector<bool> L_buf;
		std::vector<bool> R_buf;
		for (unsigned int i = 0; i < input.size(); i++) // Делим массив на 2 части
		{
			L[i] = input[i];
			R[i] = input[input.size() + i];
		}
		for (int i = 0; i < rounds_num; i++) // Проводим раунды сети Фейстеля
		{
			std::vector<bool> func_res = FeistelFunction(R, keys[i]);
			for (unsigned int j = 0; j < input.size(); j++)
			{
				L_buf[j] = R[j];
				R_buf[j] = L[j] ^ func_res[j];
				L[j] = L_buf[j];
				R[j] = R_buf[j];
			}

		}
		for (unsigned int i = 0; i < input.size(); i++) // объединяем L&R в итоговый массив
		{

			Feistel_Network_buf[i] = L[i];
			Feistel_Network_buf[i + input.size()] = R[i];
		}
		return Feistel_Network_buf;
	}
public:
	FeistelNetwork()
	{
		FeistelFunction = Xor;
	}
	FeistelNetwork(std::vector<bool>& (*F)(std::vector<bool>&, std::vector<bool>&))
	{
		if (F != NULL)
		{
			FeistelFunction = F;
		}
		else
		{
			FeistelFunction = Xor;
		}

	}
	void Change_Function(std::vector<bool>&(*F)(std::vector<bool>&, std::vector<bool>&))
	{
		if (F != NULL)
		{
			FeistelFunction = F;
		}
	}
	std::vector<bool>& encrypt(std::vector<bool>& decrypted_input_, std::vector<std::vector<bool>>& key_for_encrypt_)
	{
		Feistel_Network(decrypted_input_, key_for_encrypt_, key_for_encrypt_.size());
	}
	std::vector<bool>& decrypt(std::vector<bool>& encrypted_input_, std::vector<std::vector<bool>>& key_for_decrypt_)
	{
		Feistel_Network(encrypted_input_, key_for_decrypt_, key_for_decrypt_.size());
	}
	std::vector<bool>& encrypt(std::vector<bool>& decrypted_input_, std::vector<bool>& key_for_encrypt_)
	{
		if (key_for_encrypt_.size() % decrypted_input_.size() == 0)
		{
			std::vector<std::vector<bool>> output;
			output.resize(key_for_encrypt_.size() / decrypted_input_.size());
			for (unsigned int i = 0; i < output.size(); i++)
			{
				for (unsigned int j = 0; j < output[i].size(); j++)
				{
					output[i][j] = key_for_encrypt_[i + j];
				}

			}
			return encrypt(decrypted_input_, output);
		}
		else
		{
			std::vector<bool> error;
			return error;
		}
	}
	std::vector<bool>& decrypt(std::vector<bool>& encrypted_input_, std::vector<bool>& key_for_decrypt_)
	{
		if (key_for_decrypt_.size() % encrypted_input_.size() == 0)
		{
			std::vector<std::vector<bool>> output;
			output.resize(key_for_decrypt_.size() / encrypted_input_.size());
			for (unsigned int i = 0; i < output.size(); i++)
			{
				for (unsigned int j = 0; j < output[i].size(); j++)
				{
					output[i][j] = key_for_decrypt_[i + j];
				}

			}
			return decrypt(encrypted_input_, output);
		}
		else
		{
			return error;
		}
	}

};
class DES: public FeistelNetwork
{
private:
	std::vector<int> Start_S_table;
	std::vector<int> End_S_table;
	std::vector<int> S_table_64_56;
	std::vector<int> S_table_56_48;
	std::vector<int> E_func_table;
	std::vector<std::vector<int>> FeistelFunc_S_tables;
	std::vector<int> table_P;
	//std::vector<bool> k;
	std::vector<bool> key_64;
	std::vector<bool> key_56;
	std::vector<bool> S_block_buffer;
	std::vector<bool> CiclicLeftShift_buf;
	std::vector<std::vector<bool>> buffer_CyclShift;
	std::vector<bool> Feistel_Network_buffer;
public:
	std::vector<bool>& DeleteHexBitsFromKey(std::vector<bool> &key_64)
	{
		key_56.erase(key_56.begin(), key_56.end());
		key_56.resize(56,0);
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
	std::vector<bool>& AddHexBitsFromKey(std::vector<bool>& key_56)
	{
		key_64.erase(key_64.begin(), key_64.end());
		key_64.resize(64,0);
		int j = 0;
		for (int i = 0; i < 56; i++)
		{

			if (i % 8 != 7)
			{
				std::cerr << key_56.size() << " " << std::endl;
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
	std::vector<bool>& S_Block_common(std::vector<int>& table, std::vector<bool>& input, int size)
	{
		S_block_buffer.erase(S_block_buffer.begin(), S_block_buffer.end());
		S_block_buffer.resize(size,0);
		for (int i = 0; i < size; i++)
		{
			S_block_buffer[i] = input[table[i] - 1];
		}
		return S_block_buffer;
	}
	std::vector<bool>& S_Block(std::vector<int>& table, std::vector<bool>& input)
	{
		return S_Block_common(table, input, table.size());
	}
	std::vector<bool>& S_Block_64_56(std::vector<bool>& input)
	{
		return S_Block_common(S_table_64_56, input, 56);
	}
	std::vector<bool>& S_Block_56_48(std::vector<bool>& input)
	{
		return S_Block_common(S_table_56_48, input, 48);
	}
	std::vector<bool>& CiclicLeftShift(std::vector<bool>& input, int num) // num - величина сдвига
	{
		num = num % input.size();
		CiclicLeftShift_buf.erase(CiclicLeftShift_buf.begin(), CiclicLeftShift_buf.end());
		CiclicLeftShift_buf.resize(input.size(),0);
		for (unsigned int i = 0; i < input.size(); i++)
		{
			CiclicLeftShift_buf[i] = input[(i + num)% input.size()];
		}
		
		return CiclicLeftShift_buf;
	}
	std::vector<std::vector<bool>>& RoundKeysGeneration(std::vector<bool>& key_56)
	{
		std::vector<bool> key_added_bits = AddHexBitsFromKey(key_56);
		std::vector<bool> key_s_block = S_Block_64_56(key_added_bits);

		std::vector<std::vector<bool>> buffer_CyclShift_C;
		buffer_CyclShift_C.resize(16);
		std::vector<std::vector<bool>> buffer_CyclShift_D;
		buffer_CyclShift_D.resize(16);
		
		buffer_CyclShift.erase(buffer_CyclShift.begin(), buffer_CyclShift.end());
		buffer_CyclShift.resize(16);
		std::vector<bool> buffer_C;
		std::vector<bool> buffer_D;
		buffer_C.resize(28,0);
		buffer_D.resize(28,0);
		for (int i = 0; i < 16; i++)
		{
			buffer_CyclShift[i].resize(56,0);
			buffer_CyclShift_C[i].resize(28,0);
			buffer_CyclShift_D[i].resize(28,0);
			
			if (i == 0)
			{
				for (int j = 0; j < 28; i++)
				{
					buffer_C[j] = key_s_block[j];
					buffer_D[j] = key_s_block[j + 28];
				}
				buffer_CyclShift_C[i] = CiclicLeftShift(buffer_C, 1); //
				buffer_CyclShift_D[i] = CiclicLeftShift(buffer_D, 1);//
			}
			else if (i==1 || i==8 || i==15)
			{
				buffer_CyclShift_C[i] = CiclicLeftShift(buffer_CyclShift_C[i - 1], 1);
				buffer_CyclShift_D[i] = CiclicLeftShift(buffer_CyclShift_D[i - 1], 1);
			}
			else
			{
				
				buffer_CyclShift_C[i] = CiclicLeftShift(buffer_CyclShift_C[i - 1], 2);
				buffer_CyclShift_D[i] = CiclicLeftShift(buffer_CyclShift_D[i - 1], 2);
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

	std::vector<bool>& Start_S_Block(std::vector<bool>& input)
	{
		return S_Block(Start_S_table, input);
	}
	std::vector<bool>& End_S_Block(std::vector<bool>& input)
	{
		return S_Block(End_S_table, input);
	}

	std::vector<bool>& E_func(std::vector<bool>& input)
	{
		return S_Block(E_func_table, input);
	}
	std::vector<bool>& Feistel_func(std::vector<bool>& input, std::vector<bool>& key_48)
	{
		std::vector<bool> buffer = E_func(input);
		for (int i = 0; i < 48; i++)
		{
			buffer[i] = buffer[i] ^ key_48[i];
		}
		std::vector<bool> s_bl_buffer;
		s_bl_buffer.resize(48,0);
		for (int i=0; i<8;i++)
		{
			int str_num = buffer[i + 1] * 2 + buffer[i + 6];
			int stlb_num = buffer[i + 2] * 8 + buffer[i + 3]*4 + buffer[i+4]*2 + buffer[i+5];
			int t_num = FeistelFunc_S_tables[i][str_num * 16 + stlb_num];
			std::vector<bool> num_bit_4 = FromTenToTwo(t_num,4);
			for (int j = 0; j < 4; j++)
			{
				s_bl_buffer[i + j] = num_bit_4[j];
			}
		}

		return S_Block(table_P, s_bl_buffer);
	
	}

	std::vector<bool>& Feistel_Network(std::vector<bool>& input, std::vector<std::vector<bool>>& keys,  int rounds_num)
	{
		Feistel_Network_buffer.erase(Feistel_Network_buffer.begin(), Feistel_Network_buffer.end());
		Feistel_Network_buffer.resize(64,0);
		std::vector<bool> L;
		L.resize(32,0);
		std::vector<bool> R;
		R.resize(32, 0);
		std::vector<bool> L_buf;
		L_buf.resize(32, 0);
		std::vector<bool> R_buf;
		R_buf.resize(32, 0);
		for (int i = 0; i < 32; i++)
		{
			L[i] = input[i];
			R[i] = input[32 + i];
		}
		for (int i = 0; i < rounds_num; i++)
		{
			std::vector<bool> func_res = Feistel_func(R, keys[i]);
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
			
			Feistel_Network_buffer[i] = L[i];
			Feistel_Network_buffer[i + 32] = R[i];
		}
		return  Feistel_Network_buffer;
	}


	std::vector<bool>& encrypt(std::vector<bool>& input, std::vector<bool>& key)
	{
		std::vector<std::vector<bool>> round_keys = RoundKeysGeneration(key);
		std::vector<bool> buffer;
		buffer = Start_S_Block(input);
		buffer = Feistel_Network(buffer, round_keys,16);
		return End_S_Block(buffer);
	}
	std::vector<bool>& decrypt(std::vector<bool>& input, std::vector<bool>& key)
	{
		std::vector<std::vector<bool>> round_keys = RoundKeysGeneration(key);
		std::reverse(round_keys.begin(), round_keys.end());
		std::vector<bool> buffer;
		buffer = Start_S_Block(input);
		buffer = Feistel_Network(buffer, round_keys, 16);
		return End_S_Block(buffer);
	}
	long long encrypt(long long input, long long key)
	{
		std::vector<bool> input_bin = FromTenToTwo(input,64);
		std::vector<bool> key_bin = FromTenToTwo(key,64);
		
		std::vector<bool> buffer = encrypt(input_bin, key_bin);
		long int res = 0;
		for (unsigned int i = 0; i < buffer.size(); i++)
		{
			res = res + buffer[i] * (buffer.size() - i);
		}
		return res;
	}
	DES()
	{
		Start_S_table = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };
		End_S_table = { 40,8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55 ,	23, 63, 31,38,6, 46, 14, 54, 22, 62, 30, 37, 5, 45,	13,	53, 21,	61,	29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2,42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };
		S_table_64_56 = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3 ,60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };
		S_table_56_48 = { 14, 17, 11, 24, 1,5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };
		E_func_table = { 32,1, 2, 3, 4, 5,4, 5,6,7, 8,	9,8, 9, 10, 11, 12, 13,12, 13, 14, 15, 16, 17,16, 17, 18, 19, 20, 21,20, 21, 22, 23, 24, 25,24, 25, 26, 27, 28, 29,28, 29, 30, 31, 32, 1 };
		FeistelFunc_S_tables = {
				{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13 },
				{15,  1,  8, 14,  6, 11,  3,  4,  9 , 7,  2, 13, 12,  0,  5, 10, 3, 13,  4,  7, 15,  2,  8, 14 ,12 , 0 , 1 ,10 , 6 , 9 ,11 , 5, 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
				{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4 , 2 , 8, 13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1, 13,  6,  4,  9,  8, 15,  3 , 0 ,11 , 1 , 2 ,12,  5 ,10 ,14 , 7,  1 ,10 ,13 , 0 , 6 , 9 , 8 , 7 , 4 ,15 ,14,  3, 11,  5,  2, 12},
				{7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15, 13,  8 ,11 , 5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9, 10,  6,  9,  0, 12, 11,  7 ,13 ,15 , 1 , 3 ,14 , 5 , 2 , 8 , 4, 3 ,15 , 0 , 6 ,10,  1 ,13,  8 , 9 , 4,  5, 11, 12,  7,  2, 14},
				{2, 12,  4,  1 , 7 ,10 ,11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,  4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3 , 0 ,14,11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
				{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3 , 4, 14,  7,  5, 11, 10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,  9, 14, 15,  5,  2,  8, 12,  3 , 7 , 0 , 4 ,10 , 1 ,13, 11,  6, 4 , 3 , 2 ,12,  9,  5, 15, 10, 11, 14,  1,  7,  6 , 0 , 8, 13},
				{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1, 13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,  1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8 , 0 , 5 , 9 , 2,  6, 11, 13,  8,  1,  4 ,10 , 7 , 9 , 5 , 0 ,15, 14 , 2 , 3 ,12},
				{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7, 1, 15, 13,  8, 10,  3,  7,  4 ,12 , 5 , 6 ,11 , 0 ,14 , 9, 2,7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
		};
		table_P={ 16,  7,  20,  21,29,  12,  28,  17,1,  15,  23,  26,5,  18,  31,  10,2,  8,  24,  14,32,  27,  03,  9,19,  13,  30,  6,22,  11,  04,  25 };

	}

};
int main()
{
	DES des_chifer;
	long long int input = 0x4444444444444444;
	long long int key = 0x3333333333333333;
	long long int res = des_chifer.encrypt(input, key);
	std::cout << std::hex << res;
	
}
