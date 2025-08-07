#include "vox.h"
#include <math.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define MAX_STEPS 100

bool raycast(Field &f, glm::vec3 pos, glm::vec3 v, glm::vec3& result) {
    glm::vec3 dir = glm::normalize(v);
    // dir *= 0.2;

    pos = glm::floor(pos - f.pos);
    // std::cout << "start " << glm::to_string(pos) << std::endl;
    // std::cout << "direction " << glm::to_string(dir) << std::endl;

    for (int i = 0; i < MAX_STEPS; i++) {

        pos += v;
        // std::cout << "                            pos " << glm::to_string(pos) << std::endl;
        glm::vec3 curr = glm::floor(pos);
        if (curr.x < 0 || curr.y < 0 || curr.z < 0)
            continue;
        if (curr.x >= f.len || curr.y >= f.len || curr.z >= f.len)
            break;
        if (field_query(f, pos.x, pos.y, pos.z) > 0) {
            result = pos + f.pos;
            return true;
        }
    }
    return false;
}
