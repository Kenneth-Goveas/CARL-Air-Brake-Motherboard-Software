#ifndef __DEV_SERVO_HPP__
#define __DEV_SERVO_HPP__

#define SERVO_ADDR  0x10

namespace servo {

bool fail (void);

void put_pos (double pos);

}

#endif
