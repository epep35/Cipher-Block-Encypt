#include <iostream>
#include <string>
#include <bitset>   // For binary visualization (0101)
#include <iomanip>  // For formatting

using namespace std;

// --- HELPER FUNCTION: CIRCULAR SHIFT (ROTATION) ---
// Moves bits from the right end to the front (Wrap around)
unsigned short rotateRight(unsigned short value, int shift) {
    // Shift right 'shift' times, COMBINE with the bits pushed to the left
    return (value >> shift) | (value << (16 - shift));
}

// --- ENCRYPTION FUNCTION ---
void encryptBlock(unsigned short &L, unsigned short &R, unsigned char K) {
    cout << "\n=============================================" << endl;
    cout << "          ENCRYPTION PROCESS (DETAIL)        " << endl;
    cout << "=============================================" << endl;
    
    // DISPLAY INITIAL STATE
    cout << "INITIAL STATE:" << endl;
    cout << "Left (L) : " << bitset<16>(L) << " (Decimal: " << L << ")" << endl;
    cout << "Key  (K) : " << bitset<8>(K)  << "          (Decimal: " << (int)K << ")" << endl;
    cout << "Right(R) : " << bitset<16>(R) << " (Decimal: " << R << ")" << endl;
    cout << "---------------------------------------------" << endl;

    // STEP 1: XOR Left with Key
    // Note: The 8-bit Key is automatically padded with 0s to match 16-bit
    unsigned short step1 = L ^ K; 
    cout << "[STEP 1] Left XOR Key" << endl;
    cout << "   " << bitset<16>(L) << " (Left)" << endl;
    cout << " ^ " << bitset<16>(K) << " (Key)" << endl;
    cout << "   ----------------" << endl;
    cout << " = " << bitset<16>(step1) << endl;
    cout << "---------------------------------------------" << endl;

    // STEP 2: SHIFT 4 BITS (Circular/Rotate)
    // The bits falling off the right side reappear on the left side
    unsigned short step2 = rotateRight(step1, 4);
    cout << "[STEP 2] Shift Right 4 Bits (Circular)" << endl;
    cout << "   FROM: " << bitset<16>(step1) << endl;
    cout << "   TO  : " << bitset<16>(step2) << endl;
    cout << "   (Notice the 4 bits from the right moved to the front)" << endl;
    cout << "---------------------------------------------" << endl;

    // STEP 3: XOR with Right Side
    unsigned short noise = step2; // The result of the Left processing
    unsigned short R_new = R ^ noise;
    
    cout << "[STEP 3] XOR Result with Right Side" << endl;
    cout << "   " << bitset<16>(noise) << " (Processed Left)" << endl;
    cout << " ^ " << bitset<16>(R)     << " (Original Right)" << endl;
    cout << "   ----------------" << endl;
    cout << " = " << bitset<16>(R_new) << " (New Right)" << endl;
    cout << "---------------------------------------------" << endl;

    // UPDATE VALUES (Pad Logic)
    // L remains L (passed through the 'Pad' path)
    // R is updated to R_new
    R = R_new; 
    
    cout << "ROUND OUTPUT:" << endl;
    cout << "Left (Unchanged): " << bitset<16>(L) << endl;
    cout << "Right (Cipher)  : " << bitset<16>(R) << endl;
}

int main() {
    string textInput;
    int keyInput;

    // --- 1. USER INPUT ---
    cout << "=== DIAGRAM CIPHER SIMULATION (ENCRYPT ONLY) ===" << endl;
    cout << "Enter Text (Max 4 characters, e.g., TEST): ";
    
    // Clear buffer if necessary
    if (cin.peek() == '\n') cin.ignore(); 
    getline(cin, textInput);

    // Auto Padding (Add spaces if input is less than 4 chars)
    while (textInput.length() < 4) {
        textInput += " "; 
    }

    cout << "Enter Key (Number 0-255): ";
    cin >> keyInput;

    // --- 2. PACKING (Characters -> 16-bit Integers) ---
    // Take the first 4 characters
    unsigned char c1 = textInput[0];
    unsigned char c2 = textInput[1];
    unsigned char c3 = textInput[2];
    unsigned char c4 = textInput[3];

    // Combine them into two 16-bit blocks
    // Left Side gets Char 1 & Char 2
    unsigned short Left = (c1 << 8) | c2;  
    // Right Side gets Char 3 & Char 4
    unsigned short Right = (c3 << 8) | c4; 
    
    // Cast key to 8-bit
    unsigned char Key = (unsigned char)keyInput;

    // --- 3. EXECUTE ENCRYPTION ---
    encryptBlock(Left, Right, Key);

    // --- 4. FINAL OUTPUT ---
    cout << "\n>>> FINAL CIPHERTEXT (HEX): " 
         << hex << uppercase << Left << " " << Right << dec << endl;

    return 0;
}