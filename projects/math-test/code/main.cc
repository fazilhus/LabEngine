#include <stdio.h>
#include <vector>
#include <string>

#include "config.h"

#include "util.h"

const char* programName = "Math library";
const char* s = "OK";
const char* f = "FAILED";

typedef unsigned TestId;
typedef unsigned Line;
typedef std::string Expression;
struct FailedTest {
    TestId id;
    Line line;
    Expression expr;
};
static TestId testId = 0;
static std::vector<FailedTest> failedTests;
#define VERIFY(RESULT) {  testId++; printf("#%0*u: %*s\n", 3, testId, 6, RESULT ? s : f); if (!(RESULT)) failedTests.push_back({testId, __LINE__, #RESULT}); }

#define TEST_VIEW_PERSPECTIVE

int main()
{ 
    printf("\n\n--- %s test\n", programName);
    
    //------------------------------------------------------------------------
    const float E = 0.00001f;
    const Math::vec3 E3(E, E, E);
    const Math::vec4 E4(E, E, E, E);

    {
        printf("vec3:\n");

        // construction
        Math::vec3 v0(1.0f, 2.0f, 3.0f);
        Math::vec3 v1(4.0f, 3.0f, 2.0f);
        Math::vec3 v2(v0);
        Math::vec3 v3(v1);
        VERIFY(v0 == v2);
        VERIFY(v1 == v3);
        VERIFY(v0 != v1);
        VERIFY(v2 != v3);
        VERIFY(v0 == Math::vec3(1.0f, 2.0f, 3.0f));

        // assignment
        v2 = v1;
        VERIFY(v2 == v1);
        v2 = v0;
        VERIFY(v2 == v0);

        // operators
        // vec4 operator-() const;
        v0 = -v0;
        VERIFY(v0 == Math::vec3(-1.0f, -2.0f, -3.0f));
        v0 = -v0;
        VERIFY(v0 == v2);
        // void operator+=(const vec4& rhs);
        v2 += v3;
        VERIFY(v2 == Math::vec3(5.0f, 5.0f, 5.0f));
        // void operator-=(const vec4& rhs);
        v2 -= v3;
        VERIFY(v2 == v0);
        // void operator*=(scalar s);
        v2 *= 2.0f;
        VERIFY(v2 == Math::vec3(2.0f, 4.0f, 6.0f));
        // vec4 operator+(const vec4& rhs) const;
        v2 = v0 + v1;
        VERIFY(v2 == Math::vec3(5.0f, 5.0f, 5.0f));
        // vec4 operator-(const vec4& rhs) const;
        v2 = v0 - v1;
        VERIFY(v2 == Math::vec3(-3.0f, -1.0f, 1.0f));
        // vec4 operator*(scalar s) const;
        v2 = v0 * 2.0f;
        VERIFY(v2 == Math::vec3(2.0f, 4.0f, 6.0f));

        // setting and getting content
        v2 = Math::vec3(2.0f, 3.0f, 4.0f);
        VERIFY(v2.x == 2.0f);
        VERIFY(v2.y == 3.0f);
        VERIFY(v2.z == 4.0f);
        VERIFY(v2 == Math::vec3(2.0f, 3.0f, 4.0f));
        v2.x = 1.0f;
        v2.y = 2.0f;
        v2.z = 3.0f;
        VERIFY(v2[0] == 1.0f);
        VERIFY(v2[1] == 2.0f);
        VERIFY(v2[2] == 3.0f);
        VERIFY(v2 == Math::vec3(1.0f, 2.0f, 3.0f));
        v2.x = 5.0f;
        v2.y = 6.0f;
        v2.z = 7.0f;
        VERIFY(v2[0] == 5.0f);
        VERIFY(v2[1] == 6.0f);
        VERIFY(v2[2] == 7.0f);
        VERIFY(v2 == Math::vec3(5.0f, 6.0f, 7.0f));

        // length
        v2 = Math::vec3(1.0f, 2.0f, 3.0f);
        VERIFY(n_fequal(Math::length(v2), 3.74165738f, 0.0001f));
        
        // cross
        v0 = Math::vec3(1.0f, 0.0f, 0.0f);
        v1 = Math::vec3(0.0f, 0.0f, 1.0f);
        v2 = Math::cross(v0, v1);
        VERIFY(nearequal(v2, Math::vec3(0.0f, -1.0f, 0.0f), E3));

        // dot
        v0 = Math::vec3(1.0f, 0.0f, 0.0f);
        v1 = Math::vec3(1.0f, 0.0f, 0.0f);    
        VERIFY(Math::dot(v0, v1) == 1.0f);
        v1 = Math::vec3(-1.0f, 0.0f, 0.0f);
        VERIFY(Math::dot(v0, v1) == -1.0f);
        v1 = Math::vec3(0.0f, 1.0f, 0.0f);
        VERIFY(Math::dot(v0, v1) == 0.0f);

        // normalize
        v0 = Math::vec3(2.5f, 0.0f, 0.0f);
        v1 = Math::normalize(v0);
        VERIFY(v1 == Math::vec3(1.0f, 0.0f, 0.0f));
    }

    //------------------------------------------------------------------------
    {
        printf("vec4:\n");
        // construction
        Math::vec4 v0(1.0f, 2.0f, 3.0f, 4.0f);
        Math::vec4 v1(4.0f, 3.0f, 2.0f, 1.0f);
        Math::vec4 v2(v0);
        Math::vec4 v3(v1);
        VERIFY(v0 == v2);
        VERIFY(v1 == v3);
        VERIFY(v0 != v1);
        VERIFY(v2 != v3);
        VERIFY(v0 == Math::vec4(1.0f, 2.0f, 3.0f, 4.0));

        // assignment
        v2 = v1;
        VERIFY(v2 == v1);
        v2 = v0;
        VERIFY(v2 == v0);

        // operators
        // vec4 operator-() const;
        v0 = -v0;
        VERIFY(v0 == Math::vec4(-1.0f, -2.0f, -3.0f, -4.0f));
        v0 = -v0;
        VERIFY(v0 == v2);
        // void operator+=(const vec4& rhs);
        v2 += v3;
        VERIFY(v2 == Math::vec4(5.0f, 5.0f, 5.0f, 5.0f));
        // void operator-=(const vec4& rhs);
        v2 -= v3;
        VERIFY(v2 == v0);
        // void operator*=(scalar s);
        v2 *= 2.0f;
        VERIFY(v2 == Math::vec4(2.0f, 4.0f, 6.0f, 8.0f));
        // vec4 operator+(const vec4& rhs) const;
        v2 = v0 + v1;
        VERIFY(v2 == Math::vec4(5.0f, 5.0f, 5.0f, 5.0f));
        // vec4 operator-(const vec4& rhs) const;
        v2 = v0 - v1;
        VERIFY(v2 == Math::vec4(-3.0f, -1.0f, 1.0f, 3.0f));
        // vec4 operator*(scalar s) const;
        v2 = v0 * 2.0f;
        VERIFY(v2 == Math::vec4(2.0f, 4.0f, 6.0f, 8.0f));

        // setting and getting content
        v2 = Math::vec4(2.0f, 3.0f, 4.0f, 5.0f);
        VERIFY(v2.x == 2.0f);
        VERIFY(v2.y == 3.0f);
        VERIFY(v2.z == 4.0f);
        VERIFY(v2.w == 5.0f);
        VERIFY(v2 == Math::vec4(2.0f, 3.0f, 4.0f, 5.0f));
        v2.x = 1.0f;
        v2.y = 2.0f;
        v2.z = 3.0f;
        v2.w = 4.0f;
        VERIFY(v2[0] == 1.0f);
        VERIFY(v2[1] == 2.0f);
        VERIFY(v2[2] == 3.0f);
        VERIFY(v2[3] == 4.0f);
        VERIFY(v2 == Math::vec4(1.0f, 2.0f, 3.0f, 4.0f));
        v2.x = 5.0f;
        v2.y = 6.0f;
        v2.z = 7.0f;
        v2.w = 8.0f;
        VERIFY(v2[0] == 5.0f);
        VERIFY(v2[1] == 6.0f);
        VERIFY(v2[2] == 7.0f);
        VERIFY(v2[3] == 8.0f);
        VERIFY(v2 == Math::vec4(5.0f, 6.0f, 7.0f, 8.0f));
        
        // length
        v2 = Math::vec4(1.0f, 2.0f, 3.0f, 5.0f);
        VERIFY(n_fequal(Math::length(v2), 6.244997998f, 0.0001f));
        
        // dot
        v0 = Math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        v1 = Math::vec4(1.0f, 0.0f, 0.0f, 0.0f);    
        VERIFY(Math::dot(v0, v1) == 1.0f);
        v1 = Math::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
        VERIFY(Math::dot(v0, v1) == -1.0f);
        v1 = Math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        VERIFY(Math::dot(v0, v1) == 0.0f);

        // normalize
        v0 = Math::vec4(2.5f, 0.0f, 0.0f, 0.0f);
        v1 = Math::normalize(v0);
        VERIFY(v1 == Math::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    }

    //------------------------------------------------------------------------
    {
        printf("mat4:\n");
        const Math::vec4 pOneTwoThree(1.0, 2.0, 3.0, 1.0);
        const Math::vec3 vOneTwoThree(1.0, 2.0, 3.0);
        const Math::mat4 trans123(Math::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                            Math::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                            Math::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                            pOneTwoThree);
        const Math::mat4 identity;
        const Math::mat4 rotOneX = Math::rotationx(1.0f);
        const Math::vec4 pZero(0.0, 0.0, 0.0, 1.0);
        const Math::vec4 pOneX(1.0, 0.0, 0.0, 1.0);
        const Math::vec4 pOneY(0.0, 1.0, 0.0, 1.0);
        Math::vec4 result;

        // identity and construction
        Math::mat4 m0(identity);
        VERIFY(m0 == Math::mat4());
        VERIFY(m0 == identity);
        
        // multiply by identity
        m0 = trans123 * identity;
        VERIFY(m0 == trans123);

        // point transform by matrix
        result = trans123 * pZero;
        VERIFY((result == pOneTwoThree));

        // multiplication and multiplication order, transform point by matrix
        const Math::mat4 mRotOneX_Trans123 = trans123 * rotOneX;
        VERIFY(matnearequal(mRotOneX_Trans123,
                            Math::mat4(Math::vec4(1.0f,       0.0f,      0.0f, 0.0f),
                                 Math::vec4(0.0f,  0.540302f, 0.841471f, 0.0f),
                                 Math::vec4(0.0f, -0.841471f, 0.540302f, 0.0f),
                                 Math::vec4(1.0f,       2.0f,      3.0f, 1.0f))));
        result = mRotOneX_Trans123 * pZero;
        VERIFY(nearequal(result, pOneTwoThree, E4));
        result = mRotOneX_Trans123 * pOneX;
        VERIFY(nearequal(result, Math::vec4(2.0f, 2.0f, 3.0f, 1.0f), E4));
        result = mRotOneX_Trans123 * pOneY;
        VERIFY(nearequal(result, Math::vec4(1.0f, 2.540302f, 3.841471f, 1.0f), E4));

        // inverse
        m0 = inverse(mRotOneX_Trans123);
        VERIFY(matnearequal(m0, Math::mat4(Math::vec4( 1.0f,       0.0f,       0.0f, 0.0f),
                                     Math::vec4( 0.0f,  0.540302f, -0.841471f, 0.0f),
                                     Math::vec4( 0.0f,  0.841471f,  0.540302f, 0.0f),
                                     Math::vec4(-1.0f, -3.605018f,  0.062035f, 1.0f))));
        // transpose
        m0 = transpose(mRotOneX_Trans123);
        VERIFY(matnearequal(m0, Math::mat4(Math::vec4( 1.0f,       0.0f,       0.0f, 1.0f),
                                     Math::vec4( 0.0f,  0.540302f, -0.841471f, 2.0f),
                                     Math::vec4( 0.0f,  0.841471f,  0.540302f, 3.0f),
                                     Math::vec4( 0.0f,       0.0f,       0.0f, 1.0f))));
        // rotations
        const Math::mat4 rotX = Math::rotationx(2.0f);
        VERIFY(matnearequal(rotX, Math::mat4(Math::vec4(1.000000f,  0.000000f,  0.000000f, 0.000000f),
                                       Math::vec4(0.000000f, -0.416147f,  0.909297f, 0.000000f),
                                       Math::vec4(0.000000f, -0.909297f, -0.416147f, 0.000000f),
                                       Math::vec4(0.000000f,  0.000000f,  0.000000f, 1.000000f))));
        const Math::mat4 rotY = Math::rotationy(-1.7f);
        VERIFY(matnearequal(rotY, Math::mat4(Math::vec4(-0.128845f, 0.000000f,  0.991665f, 0.000000f),
                                       Math::vec4( 0.000000f, 1.000000f,  0.000000f, 0.000000f),
                                       Math::vec4(-0.991665f, 0.000000f, -0.128845f, 0.000000f),
                                       Math::vec4( 0.000000f, 0.000000f,  0.000000f, 1.000000f))));
        const Math::mat4 rotZ = Math::rotationz(3.1f);
        VERIFY(matnearequal(rotZ, Math::mat4(Math::vec4(-0.999135f,  0.041581f, 0.000000f, 0.000000f),
                                       Math::vec4(-0.041581f, -0.999135f, 0.000000f, 0.000000f),
                                       Math::vec4( 0.000000f,  0.000000f, 1.000000f, 0.000000f),
                                       Math::vec4( 0.000000f,  0.000000f, 0.000000f, 1.000000f))));
        const Math::vec3 rotaxis = Math::normalize(Math::vec3(1.0f, 0.2f, 2.0f));
        const Math::mat4 rot = Math::rotationaxis(rotaxis, -2.53652f);
        VERIFY(matnearequal(rot, Math::mat4(Math::vec4(-0.460861f, -0.434427f,  0.773873f, 0.000000f),
                                      Math::vec4( 0.579067f, -0.807997f, -0.108734f, 0.000000f),
                                      Math::vec4( 0.672524f,  0.398013f,  0.623936f, 0.000000f),
                                      Math::vec4( 0.000000f,  0.000000f,  0.000000f, 1.000000f))));
        
        // component-wise access
        Math::mat4 m1;
        m0 = Math::mat4(Math::vec4(2.0f, 0.0f, 0.0f, 0.0f),
                  Math::vec4(0.0f, 2.0f, 0.0f, 0.0f),
                  Math::vec4(0.0f, 0.0f, 2.0f, 0.0f),
                  Math::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        Math::vec4 value(2.0f, 0.0f, 0.0f, 0.0f);
        m1[0] = value;
        value = Math::vec4(0.0f, 2.0f, 0.0f, 0.0f);
        m1[1] = value;
        value = Math::vec4(0.0f, 0.0f, 2.0f, 0.0f);
        m1[2] = value;
        value = Math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        m1[3] = value;
        VERIFY(m0 == m1);
        VERIFY(m0[0] == Math::vec4(2.0f, 0.0f, 0.0f, 0.0f));
        VERIFY(m0[1] == Math::vec4(0.0f, 2.0f, 0.0f, 0.0f));
        VERIFY(m0[2] == Math::vec4(0.0f, 0.0f, 2.0f, 0.0f));
        VERIFY(m0[3] == Math::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        // determinant
        const float det = Math::determinant(mRotOneX_Trans123);
        VERIFY(n_fequal(1.0f, det, 0.0001f));

#ifdef TEST_VIEW_PERSPECTIVE
        const Math::vec3 eye(3.0f, 2.0f, 10.0f);
        const Math::vec3 at(3.0f, 2.0f, 2.0f);
        const Math::vec3 up(0.0f, 1.0f, 0.0f);
#ifdef USE_LH
        // lookatlh
        Math::mat4 tmp = Math::lookat(eye, at, up);
        VERIFY(matnearequal(tmp, Math::mat4(Math::vec4(  -1.0f,  0.0f,  0.0f, 0.0f),
                                      Math::vec4(  0.0f,  1.0f,  0.0f, 0.0f),
                                      Math::vec4(  0.0f, 0.0f,  -1.0f, 0.0f),
                                      Math::vec4(  3.0f, -2.0f, 10.0f, 1.0f))));
        // perspfovlh
	// fovy is expressed in radians (yes, really)
        tmp = Math::perspective(70.0f, 3.0f/4.0f, 0.1f, 50.0f);
        VERIFY(matnearequal(tmp, Math::mat4(Math::vec4( 2.814039f,     0.0f,      0.0f, 0.0f),
                                      Math::vec4(      0.0f, 2.11053f,      0.0f, 0.0f),
                                      Math::vec4(      0.0f,     0.0f, 1.004008f, 1.0f),
                                      Math::vec4(      0.0f,     0.0f,  -0.2004008f, 0.0f))));        
#else
        // lookatrh
        Math::mat4 tmp = Math::lookat(eye, at, up);
        VERIFY(matnearequal(tmp, Math::mat4(Math::vec4(  1.0f,  0.0f,  0.0f, 0.0f),
                                      Math::vec4(  0.0f,  1.0f,  0.0f, 0.0f),
                                      Math::vec4(  0.0f,  0.0f,  1.0f, 0.0f),
                                      Math::vec4(  -3.0f, -2.0f, -10.0f, 1.0f))));

        // perspfovrh
	// fovy is expressed in radians (yes, really)
        tmp = Math::perspective(70.0f, 3.0f/4.0f, 0.1f, 50.0f);
        VERIFY(matnearequal(tmp, Math::mat4(Math::vec4( 2.814039f,     0.0f,       0.0f,  0.0f),
                                      Math::vec4(      0.0f, 2.11053f,       0.0f,  0.0f),
                                      Math::vec4(      0.0f,     0.0f, -1.004008f, -1.0f),
                                      Math::vec4(      0.0f,     0.0f,  -0.2004008f,   0.0f))));
#endif
#endif
    }

    //------------------------------------------------------------------------
    // Various tests between different types

	// transform (point and vector)
    Math::mat4 m = Math::mat4(Math::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                  Math::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                  Math::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                  Math::vec4(1.0f, 2.0f, 3.0f, 1.0f));
    Math::vec4 v0 = Math::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    Math::vec4 v1 = m * v0;
    VERIFY(v1 == Math::vec4(2.0f, 2.0f, 3.0f, 1.0f));
    v0 = Math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    v1 = m * v0;
    VERIFY(v0 == Math::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    const Math::mat4 m0(Math::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                  Math::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                  Math::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                  Math::vec4(1.0f, 2.0f, 3.0f, 1.0f));
	const Math::vec4 in(0.0f, 0.0f, 0.0f, 1.0f);
	const Math::vec4 out = m0 * in;
	VERIFY(nearequal(out, Math::vec4(1.0f, 2.0f, 3.0f, 1.0f), E4));

    //------------------------------------------------------------------------
    printf("--- Done\n\n");
    if (failedTests.empty())
    {
        printf("--- %u/%u tests passed!\n\n", testId, testId);
        return 0;
    }
    for (auto t : failedTests)
        printf("Test #%u failed. Line %u, Expression: %s\n", t.id, t.line, t.expr.c_str());
    printf("--- %u/%u tests failed!\n\n", (unsigned)failedTests.size(), testId);
    return 1;
} 
