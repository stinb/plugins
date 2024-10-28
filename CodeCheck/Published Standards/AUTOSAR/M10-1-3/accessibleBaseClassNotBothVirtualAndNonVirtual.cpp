class A_1013 {};
class B1_1013: public virtual A_1013 {};
class B2_1013: public virtual A_1013 {};
class B3_1013: public A_1013 {};
class C_1013: public B1_1013, B2_1013, B3_1013 {}; 	// UndCC_Violation �
													// C has two A sub-objects		

class x_1013 {};
class y1_1013: public virtual x_1013 {};
class y2_1013: public y1_1013 {};
class z_1013: public x_1013, y2_1013 {}; 			// UndCC_Violation


class j_1013 {};
class k_1013: public j_1013 {};
class l_1013 {};
class m_1013: public virtual l_1013 {};
class n_1013: public k_1013, m_1013 {};				// UndCC_Valid
