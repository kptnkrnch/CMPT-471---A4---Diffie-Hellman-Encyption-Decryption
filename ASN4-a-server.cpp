#include <iostream>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

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

bool IsEndPacket(char * data, int size);
void DecryptPacket(int key, char * encryptedData, int slength, string * decryptedData);

int main() {
	int listensd = 0; //socket for listening for connections
	int connectionsd = 0; //socket for accepting an incoming connection
	struct sockaddr_in serv_addr; //address and port info
	int nbytes = 0; //number of bytes received
	
	listensd = socket(AF_INET, SOCK_STREAM, 0); //creating the listening socket
	memset(&serv_addr, 0, sizeof(serv_addr)); //zeroing the address information
	
	serv_addr.sin_family = AF_INET; //setting the ip version to IPv4
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //allowing any IP to connect to the server
	serv_addr.sin_port = htons(5000); //setting the port to network byte order (port 5000)
	
	bind(listensd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //binding the listening socket
	
	listen(listensd, 1); //listening on the listen socket (max 1 connection attempt at a time)
	
	connectionsd = accept(listensd, (struct sockaddr*)NULL, NULL); //setting the listening socket to wait for connections and then accept them

	string data = "HELLO WORLD THIS IS THE DATA"; //test string, unused
	string encrypted = "";
	string decrypted = "";
	BigInt n(3); //n value in the diffie-hellman protocol
	BigInt q(17); //q value (prime number) in the diffie-hellman protocol
		
	BigInt xa(rand() % 200); //generates a xa value from 0-199
	BigInt ya; //stores the value of n to the power of xa
	BigInt yb; //stores the received n to the power of xb from the client
	
	int buffer[256]; //buffer for received BigInt data, unused
	int bufsize = 256; //size of BigInt buffer
	int sendbuffInt[256]; //buffer for BigInt data to be sent
	char recvbuffChar[1024]; //buffer for received encrypted data (characters)
	int recvbuffInt[256]; //buffer for received BigInt data
	
	memset(recvbuffInt, 0, sizeof(recvbuffInt)); //zeroing recv buffer memory
	memset(recvbuffChar, 0, sizeof(recvbuffChar));  //zeroing recv char buffer memory
	memset(sendbuffInt, 0, sizeof(sendbuffInt));  //zeroing send int buffer memo
	memset(buffer, 0, sizeof(buffer)); //zeroing the receive buffer
	
	nbytes = read(connectionsd, &recvbuffInt, sizeof(recvbuffInt)); //reading data from the connected socket
		
	if (nbytes > 0 && nbytes <= bufsize * sizeof(int)) {
		yb.SetNumber(recvbuffInt, bufsize); //setting the value of yb to the received BigInt data
		
		BigInt::Copy(n, &ya); //storing the value of n into ya
		ya.Power(xa.ToInt()); //raising ya to the power of xa
		ya = BigInt(ya.Modulo(q)); //storing the value of ya mod q into ya
		
		//copying the contents of ya into the integer sending buffer
		for (int ii = 0; ii < bufsize; ii++) {
			sendbuffInt[ii] = ya.data[ii];
		}
		
		//sending the ya integer data to the client
		write(connectionsd, &sendbuffInt, sizeof(sendbuffInt)); //writing the send buffer data to the socket to the server

		BigInt k; //the container for the secret key
		BigInt::Copy(yb, &k); //copying the value of yb into k
		k.Power(xa.ToInt()); //raising k to the power of xa
		k = BigInt(k.Modulo(q)); //storing the value of k mod q into k, finalising the generation of the secret key
		
		while (true) {
			decrypted = "";
			int slength = 0;
			nbytes = read(connectionsd, &slength, sizeof(int)); //receiving the length of the string that is being sent from the client
			nbytes = read(connectionsd, &recvbuffChar, sizeof(recvbuffChar)); //receiving the encrypted data string from the client
		
			//checking to see if this the the end/terminate connection packet
			if (IsEndPacket(recvbuffChar, slength)) {
				break; //if true, break out of the loop and proceed to the close connection function
			}
			DecryptPacket(k.ToInt(), recvbuffChar, slength, &decrypted); //decrypting the encrypted data string
			cout << decrypted << endl << endl; //printing out the encrypted data
		}
	}
	
	close(connectionsd); //closing the connected socket

	return 0;
}
/* Checks to see if a received packet contains all '\0's which would indicate that it is the last packet
 */
bool IsEndPacket(char * data, int size) {
	for (int i = 0; i < size; i++) {
		if (data[i] != '\0') {
			return false;
		}
	}
	return true;
}

/* Function that decrypts the received packet using the generated secret key.
 * encryptedData is the received network data.
 * decryptedData is the string that will contain the decrypted data
 * key is the secret key
 * slength is the length of the received data string
 */
void DecryptPacket(int key, char * encryptedData, int slength, string * decryptedData) {
	for (int i = 0; i < slength; i++) {
		char c = encryptedData[i];
		c -= key * 2;
		c = c^key;
		*decryptedData += c;
	}
}
