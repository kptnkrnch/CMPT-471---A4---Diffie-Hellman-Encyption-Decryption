#include <iostream>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <math.h>
#include <string.h>

using namespace std;

/* BigInt class, used to store large integers that would otherwise not fit into
 * a standard integer data type.
 * Provides some mathematical functions such as addition, subtraction,
 * multiplication, and modulo.
 * 
 * data array, size is 256 ints
 * each index in the data array represents a number from 0-9
 * ie, 256 would look like [0][0][0]...[0][0][2][5][6]
 */
class BigInt {
public:
	int data[256]; //container for representing large numbers
	static const int size = 256; //size of the data array

	/*Default Constructor, initializes the data array to all 0s*/
	BigInt() {
		for (int i = 0; i < size; i++) {
			data[i] = 0;
		}
	}

	/*Constructor that sets the data array to equal a number
	 *IE, BigInt(5) - 0000...005
	 */
	BigInt(int number) {
		for (int i = 0; i < size; i++) {
			data[i] = 0;
		}

		if (number != 0) {
			string temp = "";
			stringstream ss;
			ss << number;
			temp = ss.str();

			if (temp.size() <= size) {
				int index = size - temp.size();
				for (int i = 0; index < size; i++, index++) {
					char num = temp.at(i);
					data[index] = atoi(&num);
				}
			}
		}
	}

	/*Reinitializes the data array to all 0s*/
	void Init() {
		for (int i = 0; i < size; i++) {
			data[i] = 0;
		}
	}

	/*Copies the data contents of the source BigInt into the destination BigInt (pointer)*/
	static void Copy(BigInt source, BigInt * destination) {
		for (int i = 0; i < size; i++) {
			destination->data[i] = source.data[i];
		}
	}

	/*Sets the current BigInt to the desired number*/
	void SetNumber(int number) {
		Init();
		if (number != 0) {
			string temp = "";
			stringstream ss;
			ss << number;
			temp = ss.str();

			if (temp.size() <= size) {
				int index = size - temp.size();
				for (int i = 0; index < size; i++, index++) {
					char num = temp.at(i);
					data[index] = atoi(&num);
				}
			}
		}
	}

	/*Sets the BigInt that it is called on to the number specified*/
	void SetNumber(int * number, int nsize) {
		Init();
		if (nsize == size) {
			for (int i = 0; i < size; i++) {
				data[i] = number[i];
			}
		}
	}

	/*Takes the current BigInt's data value and raises it to the power of the desired exponent*/
	void Power(int exponent) {
		BigInt temp;
		Copy(*this, &temp);
		if (exponent == 0) {
			temp.SetNumber(1);
		}
		for (int i = 0; i < exponent - 1; i++) {
			temp.Multiply(*this);
			//temp.PrintNumber();
		}
		Copy(temp, this);
	}

	/*Takes the current BigInt's data value and multiplies it by the desired
	  BigInt in the number field, this value is stored in the current BigInt*/
	void Multiply(BigInt number) {
		BigInt numbers[size];
		for (int i = 0; i < size; i++) {
			numbers[i].Init();
		}

		for (int n = 0; n < size; n++) {
			for (int i = size - 1; i >= 0; i--) {
				int temp = number.data[(size - 1 - n)] * data[i];
				if (temp < 10 && temp != 0) {
					int addtemp = temp + numbers[n].data[i - n];
					if (addtemp < 10) {
						numbers[n].data[i - n] = addtemp;
					} else {
						for (int tempIa = i - 1 - n; tempIa >= 0; tempIa--) {
							int temp1a = ((addtemp - (addtemp % 10))/10);
							int temp2a = (addtemp % 10);
						
							numbers[n].data[tempIa + 1] = temp2a;
							addtemp = numbers[n].data[tempIa] + temp1a;
							if (addtemp < 10) {
								numbers[n].data[tempIa] = addtemp;
								break;
							}
						}
					}
				} else {
					for (int tempI = i - 1 - n; tempI >= 0; tempI--) {
						int temp1 = ((temp - (temp % 10))/10);
						int temp2 = (temp % 10);
						
						int addtemp = numbers[n].data[tempI + 1] + temp2;
						if (addtemp < 10) {
							numbers[n].data[tempI + 1] = addtemp;
						} else {
							for (int tempIa = tempI; tempIa >= 0; tempIa--) {
								int temp1a = ((addtemp - (addtemp % 10))/10);
								int temp2a = (addtemp % 10);
						
								numbers[n].data[tempIa + 1] = temp2a;
								addtemp = numbers[n].data[tempIa] + temp1a;
								if (addtemp < 10) {
									numbers[n].data[tempIa] = addtemp;
									break;
								}
							}
						}

						temp = numbers[n].data[tempI] + temp1;
						if (temp < 10) {
							numbers[n].data[tempI] = temp;
							break;
						}
					}
				}
			}
		}

		Init();
		for (int i = 0; i < size; i++) {
			Add(numbers[i]);
		}
	}

	/*FOR TESTING PURPOSES, DEPRECATED*/
	void Multiply2(BigInt number) {
		BigInt numbers[size];
		for (int i = 0; i < size; i++) {
			numbers[i].Init();
		}
		PrintNumber();
		cout << "--------------------------------------------------------" << endl;
		for (int n = 0; n < size; n++) {
			for (int i = size - 1; i >= 0; i--) {
				int temp = number.data[(size - 1 - n)] * data[i];
				if (temp < 10 && temp != 0) {
					int addtemp = temp + numbers[n].data[i - n];
					if (addtemp < 10) {
						numbers[n].data[i - n] = addtemp;
					} else {
						for (int tempIa = i - 1 - n; tempIa >= 0; tempIa--) {
							int temp1a = ((addtemp - (addtemp % 10))/10);
							int temp2a = (addtemp % 10);
						
							numbers[n].data[tempIa + 1] = temp2a;
							addtemp = numbers[n].data[tempIa] + temp1a;
							if (addtemp < 10) {
								numbers[n].data[tempIa] = addtemp;
								break;
							}
						}
					}
				} else {
					for (int tempI = i - 1 - n; tempI >= 0; tempI--) {
						int temp1 = ((temp - (temp % 10))/10);
						int temp2 = (temp % 10);
						
						int addtemp = numbers[n].data[tempI + 1] + temp2;
						if (addtemp < 10) {
							numbers[n].data[tempI + 1] = addtemp;
						} else {
							for (int tempIa = tempI; tempIa >= 0; tempIa--) {
								int temp1a = ((addtemp - (addtemp % 10))/10);
								int temp2a = (addtemp % 10);
						
								numbers[n].data[tempIa + 1] = temp2a;
								addtemp = numbers[n].data[tempIa] + temp1a;
								if (addtemp < 10) {
									numbers[n].data[tempIa] = addtemp;
									break;
								}
							}
						}

						temp = numbers[n].data[tempI] + temp1;
						if (temp < 10) {
							numbers[n].data[tempI] = temp;
							break;
						}
					}
				}
			}
		}

		Init();
		for (int i = 0; i < 10; i++) {
			numbers[i].PrintNumber();
			Add(numbers[i]);
		}
	}

	/*Takes the current BigInt and adds the number BigInt to it,
	  this data is automatically stored in the current BigInt*/
	void Add(BigInt number) {
		if (number.size == size) {
			for (int i = size - 1; i >= 0; i--) {
				int temp = number.data[i] + data[i];
				if (temp < 10) {
					data[i] = temp;
				} else {
					for (int tempI = i - 1; tempI >= 0; tempI--) {
						int temp1 = ((temp - (temp % 10))/10);
						int temp2 = (temp % 10);
						
						data[tempI + 1] = temp2;
						temp = data[tempI] + temp1;
						if (temp < 10) {
							data[tempI] = temp;
							break;
						}
					}
				}
			}
		}
	}

	/*Takes the current BigInt and subtracts the number BigInt from it,
	  this data is automatically stored in the current BigInt*/
	void Subtract(BigInt number) {
		for (int i = size - 1; i >= 0; i--) {
			int temp = data[i];
			if (temp >= number.data[i]) {
				data[i] = temp - number.data[i];
			} else {
				for (int tempI = i - 1; tempI >= 0; tempI--) {
					if (data[tempI] > 0) {
						data[tempI] -= 1;
						for (tempI = tempI + 1; tempI < i; tempI++) {
							data[tempI] += 9;
						}
						data[i] += 10;
						break;
					}
				}
				data[i] -= number.data[i];
			}
		}
	}

	/*Old Modulo function, deprecated*/
	void Mod(BigInt number) {
		BigInt temp;
		Copy(*this, &temp);
		while (!temp.IsNegative()) {
			temp.Subtract(number);
			if (!temp.IsNegative()) {
				Copy(temp, this);
			}
		}
	}

	/*Takes the current BigInt and mods it with "number" and returns
	  an integer containing the calculated mod value*/
	int Modulo(BigInt number) {
		BigInt div;
		int mod = 0;
		int num = number.ToInt();
		int length = 0;
		for (; length < size; length++) {
			if (number.data[length] != 0) {
				break;
			}
		}
		length = size - length;
		int index = 0;
		for (; index < size; index++) {
			if (data[index] != 0) {
				break;
			}
		}
		if (index < size) {
			bool first = true;
			if (index > (size - length)) {
				mod = data[index];
			}
			for (; index <= size - length; index++) {
				int tempNum = 0;
				for (int i = length - 1; i >= 0; i--) {
					if (first) {
						tempNum += data[index + (length - i - 1)] * (int)pow((double)10, i);
						first = false;
					} else if (i == length - 1) {
						tempNum += mod * (int)pow((double)10, i);
					} else {
						tempNum += data[index + (length - i - 1)] * (int)pow((double)10, i);
					}
				}
				int modNum = tempNum % num;
				int divNum = tempNum / num;

				mod = modNum;
			}
		}
		return mod;
	}

	/*Used for creating a new BigInt number, the integer number is converted and
	  stored in the bigint pointer*/
	static void CreateNumber(int number, BigInt * bigint) {
		string temp = "";
		stringstream ss;
		ss << number;
		temp = ss.str();

		if (temp.size() <= bigint->size) {
			int index = bigint->size - temp.size();
			for (int i = 0; index < bigint->size; i++, index++) {
				char num = temp.at(i);
				bigint->data[index] = atoi(&num);
			}
		}
	}

	/*Prints out the value of the number*/
	void PrintNumber() {
		for (int i = 0; i < size; i++) {
			cout << data[i];
		}
		cout << endl;
	}

	/*Because BigInts in this class can only ever be positive,
	  this checks if the currrent BigInt has been corrupted with negative values*/
	bool IsNegative() {
		for (int i = 0; i < size; i++) {
			if (data[i] < 0) {
				return true;
			}
		}
		return false;
	}

	/*Converts the current BigInt into a standard integer value,
	  this integer value is returned by this function*/
	int ToInt() {
		int number = 0;
		for (int i = size - 1, n = 0; i >= 0; i--, n++) {
			number += data[i] * (int)pow((double)10, n);
		}
		return number;
	}

	/*Checks if two BigInts are equal to each other, used for testing purposes.*/
	static bool Compare(BigInt a, BigInt b) {
		for (int i = 0; i < size; i++) {
			if (a.data[i] != b.data[i]) {
				return false;
			}
		}
		return true;
	}
};

void SendEndPacket(int sd);
void EncryptPacket(int key, string data, string * encryptedData);

int main(int argc, char *argv[]) {
	int sd = 0;
	int port = 5000;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	BigInt xa;
	
	if (argc >= 2) {
		//memset(sendbuff, 0, sizeof(sendbuff)); //zeroing send buffer memory
	
		sd = socket(AF_INET, SOCK_STREAM, 0); //creating the TCP socket
		server = gethostbyname(argv[1]); //getting the server information from the ip string
		
		serv_addr.sin_family = AF_INET; //setting the ip version to IPv4
		
		memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, sizeof(server->h_addr)); //copying the server address from the hostent struct to the sockaddr_in struct
		serv_addr.sin_port = htons(port); //setting the port to network byte order
		
		connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); //connecting the the server
		
		BigInt xa = BigInt(rand() % 200);
		
		string data[10];
		data[0] = "HELLO WORLD THIS IS AN ENCRYPTION TEST";
		data[1] = "LFTR is a type of thorium molten salt reactor (TMSR). Molten-salt-fueled reactors (MSRs) supply the nuclear fuel in the form of a molten salt mixture.";
		data[2] = "In a LFTR, thorium and uranium-233 are dissolved in carrier salts, forming a liquid fuel.";
		data[3] = "The LFTR needs a mechanism to remove the fission products from the fuel salt and recover at least the fissile material. Some fission products in the salt absorb neutrons and reduce the production of new fissile fuel. ";
		data[4] = "The LFTR resists diversion of its fuel to nuclear weapons in four ways: first, the thorium-232 breeds by converting first to protactinium-233, which then decays to uranium-233. If the protactinium remains in the reactor, small amounts of U-232 are also produced. U-232 has a decay chain product (thallium-208) that emits powerful, dangerous gamma rays. These are not a problem inside a reactor, but in a bomb, they complicate bomb manufacture, harm electronics and reveal the bomb's location.";
		data[5] = "Conventional reactors consume less than one percent of the mined uranium, leaving the rest as waste. With well working reprocessing LFTR may consume about 99% of its thorium fuel. The improved fuel efficiency means that 1 ton of natural thorium in a LFTR produces as much energy as 35 t of enriched uranium in conventional reactors (requiring 250 t of natural uranium), or 4,166,000 tons of black coal in a coal power plant.";
		data[6] = "Fluoride salt mixtures have melting points ranging from 300 to over 600 degrees Celsius. The salts, especially those with beryllium fluoride, are very viscous near their freezing point. This requires careful design and freeze protection in the containment and heat exchangers. Freezing must be prevented in normal operation, during transients, and during extended downtime.";
		data[7] = "The FUJI MSR was a design for a 100 to 200 MWe molten-salt-fueled thorium fuel cycle thermal breeder reactor, using technology similar to the Oak Ridge National Laboratory Reactor Experiment. It was being developed by a consortium including members from Japan, the United States, and Russia. As a breeder reactor, it converts thorium into nuclear fuels.";
		data[8] = "Kirk Sorensen, former NASA scientist and Chief Nuclear Technologist at Teledyne Brown Engineering, has been a long time promoter of thorium fuel cycle and particularly liquid fluoride thorium reactors. He first researched thorium reactors while working at NASA, while evaluating power plant designs suitable for lunar colonies.";
		data[9] = "The People's Republic of China has initiated a research and development project in thorium molten-salt reactor technology. It was formally announced at the Chinese Academy of Sciences (CAS) annual conference in January 2011. Its ultimate target is to investigate and develop a thorium based molten salt nuclear system in about 20 years. An expected intermediate outcome of the TMSR research program is to build a 2 MW pebble bed fluoride salt cooled research reactor in 2015, and a 2 MW molten salt fueled research reactor in 2017.";
		string encrypted = "";
		
		BigInt n(3);
		BigInt q(17);
	
		BigInt xb(0);
		
		int sendbuffInt[256];
		char sendbuffChar[1024];
		int bufsizeChar = 1024;
		int bufsize = 256;
		int nbytes = 0;
		int recvbuffInt[256];
		memset(recvbuffInt, 0, sizeof(recvbuffInt)); //zeroing recv buffer memory
		memset(sendbuffChar, 0, sizeof(sendbuffChar));  //zeroing send char buffer memory
		memset(sendbuffInt, 0, sizeof(sendbuffInt));  //zeroing send int buffer memory
		
		BigInt ya;
		BigInt yb;
		BigInt k;
		BigInt::Copy(n, &ya);
		ya.Power(xa.ToInt());
		ya = BigInt(ya.Modulo(q));
		
		for (int ii = 0; ii < bufsize; ii++) {
			sendbuffInt[ii] = ya.data[ii];
		}
		
		write(sd, &sendbuffInt, sizeof(sendbuffInt)); //writing the send buffer data to the socket to the server
		
		nbytes = read(sd, &recvbuffInt, sizeof(recvbuffInt));
		
		if (nbytes > 0 && nbytes <= sizeof(recvbuffInt)) {
			yb.SetNumber(recvbuffInt, 256);
			
			BigInt::Copy(yb, &k);
			k.Power(xa.ToInt());
			k = BigInt(k.Modulo(q));
			
			/*for (int i = 0; i < data.size(); i++) {
				char c = data.at(i) + k.ToInt();
				encrypted += c;
			}*/
			for (int zz = 0; zz < 10; zz++) {
				encrypted = "";
				EncryptPacket(k.ToInt(), data[zz], &encrypted);
			
				strcpy(sendbuffChar, encrypted.c_str());
				int slength = encrypted.size();
				write(sd, &slength, sizeof(int));
				write(sd, &sendbuffChar, sizeof(sendbuffChar));
			
				cout << encrypted << endl << endl;
			}
			
			SendEndPacket(sd);
		}
		//ka.PrintNumber();
		//cout << ka.ToInt() << endl;

		//BigInt kb;
		//BigInt::Copy(ya, &kb);
		//kb.Power(xb.ToInt());
		//kb = BigInt(kb.Modulo(q));
		//kb.PrintNumber();
		//cout << kb.ToInt() << endl;

		/*for (int i = 0; i < encrypted.size(); i++) {
			char c = encrypted.at(i) - kb.ToInt();
			decrypted += c;
		}
		cout << decrypted << endl;*/
		
		close(sd); //closing the socket
	}

	return 0;
}

void SendEndPacket(int sd) {
	int size = 256;
	char data[256];
	for (int i = 0; i < size; i++) {
		data[i] = '\0';
	}
	write(sd, &size, sizeof(int));
	write(sd, &data, sizeof(data));
}

void EncryptPacket(int key, string data, string * encryptedData) {
	for (int i = 0; i < data.size(); i++) {
		char c = data.at(i);
		c = c^key;
		c += key * 2;
		*encryptedData += c;
	}
}
