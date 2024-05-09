#ifndef MODE_HELPERS
#define MODE_HELPERS

void setNthBit(uint8_t* array, int index, bool value) {
    if (value) {
        array[index / 8] |= (1 << (index % 8));
    } else {
        array[index / 8] &= ~(1 << (index % 8));
    }
}

uint32_t brightnessToUint32(uint8_t brightness) {
    uint32_t calculatedValue;
    switch (brightness) {
        case 0:
            calculatedValue = 0;
            break;
        case 1:
            calculatedValue = 0x1;
            break;
        case 2:
            calculatedValue = 0x10001;
            break;
        case 3:
            calculatedValue = 0x200801;
            break;
        case 4:
            calculatedValue = 0x1010101;
            break;
        case 5:
            calculatedValue = 0x4102041;
            break;
        case 6:
            calculatedValue = 0x2108421;
            break;
        case 7:
            calculatedValue = 0x1111111;
            break;
        case 8:
            calculatedValue = 0x11111111;
            break;
        case 9:
            calculatedValue = 0x12491111;
            break;
        case 10:
            calculatedValue = 0x9249249;
            break;
        case 11:
            calculatedValue = 0x49249249;
            break;
        case 12:
            calculatedValue = 0x55249249;
            break;
        case 13:
            calculatedValue = 0x55549249;
            break;
        case 14:
            calculatedValue = 0x55555249;
            break;
        case 15:
            calculatedValue = 0x55555549;
            break;
        case 16:
            calculatedValue = 0x55555555;
            break;
        case 17:
            calculatedValue = 0x55555557;
            break;
        case 18:
            calculatedValue = 0x5555555F;
            break;
        case 19:
            calculatedValue = 0x5555557F;
            break;
        case 20:
            calculatedValue = 0x555555FF;
            break;
        case 21:
            calculatedValue = 0x555557FF;
            break;
        case 22:
            calculatedValue = 0x55555FFF;
            break;
        case 23:
            calculatedValue = 0x55557FFF;
            break;
        case 24:
            calculatedValue = 0x5555FFFF;
            break;
        case 25:
            calculatedValue = 0x5557FFFF;
            break;
        case 26:
            calculatedValue = 0x555FFFFF;
            break;
        case 27:
            calculatedValue = 0x557FFFFF;
            break;
        case 28:
            calculatedValue = 0x55FFFFFF;
            break;
        case 29:
            calculatedValue = 0x57FFFFFF;
            break;
        case 30:
            calculatedValue = 0x5FFFFFFF;
            break;
        case 31:
            calculatedValue = 0x7FFFFFFF;
            break;
        case 32:
            calculatedValue = 0xFFFFFFFF;
            break;
        default:
            calculatedValue = 0x0;
            break;
    }
    return calculatedValue;
}

#endif