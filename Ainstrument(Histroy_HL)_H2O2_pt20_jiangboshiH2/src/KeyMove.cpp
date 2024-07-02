/*
 * KeyMove.cpp
 *
 *  Created on: 2020Äê2ÔÂ5ÈÕ
 *      Author: Administrator
 */

#include "KeyMove.h"
#include "Anjian.h"

KeyMove::KeyMove() {
	// TODO Auto-generated constructor stub

}
void KeyMove::move(u8 m_show, int &m_curItem, u32 jian, bool &m_once) {

	switch (m_show) {
	case 2:
		if (jian == UP) {
			m_once = true;
			if(m_curItem==-1)
				m_curItem=5;
			else
				m_curItem=(m_curItem+5)%6;
/*
			switch (m_curItem) {
			case 0:
				m_curItem = 3;
				break;
			case 1:
			case 2:
			case 3:
				m_curItem--;
				break;
			case 4:
				m_curItem = 5;
				break;
			case 5:
				m_curItem = 4;
				break;
			default:
				m_curItem = 5;
				break;
			}
*/

		} else if (jian == DOWN) {
			m_once = true;
			m_curItem=(m_curItem+1)%6;
/*
			switch (m_curItem) {
			case 3:
				m_curItem = 0;
				break;
			case 0:
			case 1:
			case 2:
				m_curItem++;
				break;
			case 4:
				m_curItem = 5;
				break;
			case 5:
				m_curItem = 4;
				break;
			default:
				m_curItem = 0;
				break;
			}
*/
		} else if (jian == RIGHT || jian == LEFT) {
			m_once = true;
			switch (m_curItem) {
			case 0:
			case 1:
			case 2:
				m_curItem = 4;
				break;
			case 3:
				m_curItem = 5;
				break;
			case 4:
				m_curItem = 0;
				break;
			case 5:
				m_curItem = 3;
				break;
			default:
				m_curItem = 0;
				break;
			}
		}
		break;
	case 3:
		if (jian == UP) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 4;
			else if (m_curItem < 2)
				m_curItem = (m_curItem + 1) % 2;
			else if (m_curItem < 5)
				m_curItem = 2 + (m_curItem - 2 + 2) % 3;
		} else if (jian == DOWN) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else if (m_curItem < 2)
				m_curItem = (m_curItem + 1) % 2;
			else if (m_curItem < 5)
				m_curItem = 2 + (m_curItem - 2 + 1) % 3;
		} else if (jian == RIGHT || jian == LEFT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else if (m_curItem < 2)
				m_curItem += 2;
			else if (m_curItem < 4)
				m_curItem -= 2;
			else if (m_curItem == 4)
				m_curItem = 1;
		}
		break;
	case 15:
		if (jian == UP||jian == DOWN) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 4;
			else if (m_curItem ==3)
				m_curItem = 4;
			else if (m_curItem ==4)
				m_curItem = 3;
		} else if (jian == RIGHT || jian == LEFT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 4;
			else if (m_curItem ==1)
				m_curItem=3;
			else if (m_curItem >2 )
				m_curItem = 1;
		}
		break;
	case 4:
		if (jian == UP) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 11;
			else
				m_curItem = (m_curItem + 8) % 12;
		} else if (jian == DOWN) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else
				m_curItem = (m_curItem + 4) % 12;
		} else if (jian == RIGHT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else {
				m_curItem = (m_curItem + 1) % 12;
				if (m_curItem % 4 == 0)
					m_curItem = (m_curItem + 8) % 12;
			}
		} else if (jian == LEFT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 11;
			else {
				if (m_curItem % 4 == 0)
					m_curItem = m_curItem + 3;
				else
					m_curItem = (m_curItem - 1) % 12;
			}
		}
		break;

		//	case 4:
		//		if (jian == UP) {
		//			m_once = true;
		//		} else if (jian == DOWN) {
		//			m_once = true;
		//		} else if (jian == RIGHT) {
		//			m_once = true;
		//		} else if (jian == LEFT) {
		//			m_once = true;
		//		}
		//		break;
	case 6:
		if (jian == UP) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 15;
			else if (m_curItem > 3)
				m_curItem -= 4;
			else
				m_curItem += 12;

		} else if (jian == DOWN) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else if (m_curItem < 12)
				m_curItem += 4;
			else
				m_curItem -= 12;

		} else if (jian == RIGHT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else
				m_curItem = (m_curItem + 1) % 16;
			if (m_curItem % 4 == 0)
				m_curItem = (m_curItem + 12) % 16;
		} else if (jian == LEFT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 15;
			else if (m_curItem % 4 == 0)
				m_curItem += 3;
			else
				m_curItem--;
		}
		break;

	case 7:
		if (jian == UP) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 14;
			else if (m_curItem < 3)
				m_curItem += 9;
			else if (m_curItem < 12)
				m_curItem = m_curItem - 3;
			else if (m_curItem == 12)
				m_curItem = 14;
			else if (m_curItem == 13)
				m_curItem = 12;
			else if (m_curItem == 14)
				m_curItem = 13;

		} else if (jian == DOWN) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else if (m_curItem == 14)
				m_curItem = 12;
			else if (m_curItem == 13)
				m_curItem = 14;
			else if (m_curItem == 12)
				m_curItem = 13;
			else if (m_curItem >= 9)
				m_curItem = m_curItem - 9;
			else if (m_curItem < 9)
				m_curItem += 3;
		} else if (jian == RIGHT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else if (m_curItem == 2)
				m_curItem = 12;
			else if (m_curItem == 5 || m_curItem == 8)
				m_curItem = 13;
			else if (m_curItem == 11)
				m_curItem = 14;
			else if (m_curItem == 12)
				m_curItem = 0;
			else if (m_curItem == 13)
				m_curItem = 3;
			else if (m_curItem == 14)
				m_curItem = 9;
			else if (m_curItem % 3 == 0 || m_curItem % 3 == 1)
				m_curItem++;
		} else if (jian == LEFT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 12;
			else if (m_curItem == 0)
				m_curItem = 12;
			else if (m_curItem == 3 || m_curItem == 6)
				m_curItem = 13;
			else if (m_curItem == 9)
				m_curItem = 14;
			else if (m_curItem == 12)
				m_curItem = 2;
			else if (m_curItem == 13)
				m_curItem = 5;
			else if (m_curItem == 14)
				m_curItem = 11;
			else if (m_curItem % 3 == 1 || m_curItem % 3 == 2)
				m_curItem--;
		}
		break;
	case 10:
		if (jian == UP) {
			m_once = true;
			switch (m_curItem) {
			case 0:
				m_curItem = 5;
				break;
			case 1:
				m_curItem = 9;
				break;
			case 2:
			case 3:
			case 4:
				m_curItem = 0;
				break;
			case 5:
			case 6:
			case 7:
				m_curItem -= 3;
				break;
			case 8:
			case 9:
				m_curItem = 1;
				break;
			default:
				m_curItem = 9;
				break;
			}
		} else if (jian == DOWN) {
			m_once = true;
			switch (m_curItem) {
			case 0:
				m_curItem = 2;
				break;
			case 1:
				m_curItem = 9;
				break;
			case 2:
			case 3:
			case 4:
				m_curItem += 3;
				break;
			case 5:
			case 6:
			case 7:
				m_curItem = 0;
				break;
			case 8:
			case 9:
				m_curItem = 1;
				break;
			default:
				m_curItem = 2;
				break;
			}
		} else if (jian == RIGHT) {
			m_once = true;
			switch (m_curItem) {
			case 0:
				m_curItem = 1;
				break;
			case 1:
				m_curItem = 0;
				break;
			case 2:
			case 3:
			case 5:
			case 6:
			case 7:
			case 8:
				m_curItem++;
				break;
			case 4:
				m_curItem = 8;
				break;
			case 9:
				m_curItem = 5;
				break;
			default:
				m_curItem = 2;
				break;
			}
		} else if (jian == LEFT) {
			m_once = true;
			switch (m_curItem) {
			case 0:
				m_curItem = 1;
				break;
			case 1:
				m_curItem = 0;
				break;
			case 2:
			case 5:
				m_curItem = 9;
				break;
			case 3:
			case 4:
			case 6:
			case 7:
			case 8:
			case 9:
				m_curItem--;
				break;
			default:
				m_curItem = 9;
				break;
			}
		}
		break;
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		if (jian == UP) {
			if (m_curItem < 0) {
				m_curItem = 0;
				m_once = true;
			}
		} else if (jian == DOWN) {
			if (m_curItem < 0) {
				m_curItem = 0;
				m_once = true;
			}
		} else if (jian == RIGHT || jian == LEFT) {
			m_once = true;
			if (m_curItem < 0)
				m_curItem = 0;
			else
				m_curItem = (m_curItem + 1) % 2;
		}
		break;
	default:
		break;
	}
}
