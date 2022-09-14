#ifndef OPENGLPROJECT_CUBETREE_H
#define OPENGLPROJECT_CUBETREE_H

#include <vector>
#include <glm/common.hpp>
#include "common/abstracts/BoundBox.h"
#include "common/WorldObject.h"
#include "common/primitives/BoundBoxRenderer.h"

template<typename T>
class CubeTreeUnit
{
private:
    glm::vec3 position;
    T data;
public:
    CubeTreeUnit(const glm::vec3 &position, const T &data): position(position), data(data){}
    const T &getData() const {return data;}
    const glm::vec3 &getPosition() const {return position;}
};
template<typename T>
class CubeTree
{
private:
    std::vector<CubeTreeUnit<T>> units;
    int capacity = 4;
    BoundBox bb;
    bool is_divided = false;

    CubeTree<T> *left_lower_bottom = nullptr;
    CubeTree<T> *right_lower_bottom = nullptr;
    CubeTree<T> *right_lower_front = nullptr;
    CubeTree<T> *left_lower_front= nullptr;

    CubeTree<T> *left_upper_bottom = nullptr;
    CubeTree<T> *right_upper_bottom = nullptr;
    CubeTree<T> *right_upper_front = nullptr;
    CubeTree<T> *left_upper_front= nullptr;
    void subdivide();
public:
    CubeTree<T> *getLeftBottomBack() const {return left_lower_bottom;}
    CubeTree<T> *getRightBottomBack() const{return right_lower_bottom;}
    CubeTree<T> *getRightBottomFront() const{return right_lower_front;}
    CubeTree<T> *getLeftBottomFront() const{return left_lower_front;}

    CubeTree<T> *getLeftUpperBack() const{return left_upper_bottom;}
    CubeTree<T> *getRightUpperBack() const{return right_upper_bottom;}
    CubeTree<T> *getRightUpperFront() const{return right_upper_front;}
    CubeTree<T> *getLeftUpperFront() const{return left_upper_front;}
    const BoundBox &getBoundBox() const {return bb; }
    bool isDivided() const {return is_divided;}
    CubeTree(const BoundBox &bb);
    bool insert(const CubeTreeUnit<T> &unit);
    void getData(const BoundBox &bb, std::vector<CubeTreeUnit<T>> &data);
};
template<typename T>
void CubeTree<T>::subdivide()
{
    is_divided = true;

    float hx = bb.getSizeX() * 0.5f;
    float hy = bb.getSizeY() * 0.5f;
    float hz = bb.getSizeZ() * 0.5f;

    glm::vec3 p0 = bb.getLeftBottomBack() + glm::vec3(hx, 0, 0);
    glm::vec3 p1 = bb.getRightBottomFront() - glm::vec3(0, 0, hz);
    glm::vec3 p2 = bb.getRightBottomFront() - glm::vec3(hx, 0, 0);
    glm::vec3 p3 = bb.getLeftBottomBack() + glm::vec3(0, 0, hz);

    glm::vec3 p4 = bb.getLeftUpperBack() + glm::vec3(hx, 0, 0);
    glm::vec3 p5 = bb.getRightUpperFront() - glm::vec3(0, 0, hz);
    glm::vec3 p6 = bb.getRightUpperFront() - glm::vec3(hx, 0, 0);
    glm::vec3 p7 = bb.getLeftUpperBack() + glm::vec3(0, 0, hz);

    glm::vec3 p8 = bb.getLeftBottomBack() + glm::vec3(0, hy, 0);
    glm::vec3 p9 = bb.getRightBottomBack() + glm::vec3(0, hy, 0);
    glm::vec3 p10 = bb.getRightBottomFront() + glm::vec3(0, hy, 0);
    glm::vec3 p11 = bb.getLeftBottomFront() + glm::vec3(0, hy, 0);

    glm::vec3 center = bb.getLeftBottomBack() + glm::vec3(hx, hy, hz);
    glm::vec3 center_right = center + glm::vec3(hx, 0, 0);
    glm::vec3 center_left = center - glm::vec3(hx, 0, 0);
    glm::vec3 center_bottom = center - glm::vec3(0, hy, 0);
    glm::vec3 center_upper = center + glm::vec3(0, hy, 0);
    glm::vec3 center_front = center + glm::vec3(0, 0, hz);
    glm::vec3 center_back = center - glm::vec3(0, 0, hz);

    left_lower_bottom = new CubeTree(BoundBox(bb.getLeftBottomBack(), center));
    right_lower_bottom = new CubeTree(BoundBox(p0, center_right));
    right_lower_front = new CubeTree(BoundBox(center_bottom, p10));
    left_lower_front= new CubeTree(BoundBox(p3, center_front));

    left_upper_bottom = new CubeTree(BoundBox(p8, center_upper));
    right_upper_bottom = new CubeTree(BoundBox(center_back, p5));
    right_upper_front = new CubeTree(BoundBox(center, bb.getRightUpperFront()));
    left_upper_front= new CubeTree(BoundBox(center_left, p6));
}
template<typename T>
CubeTree<T>::CubeTree(const BoundBox &bb) : bb(bb)
{
}
template<typename T>
bool CubeTree<T>::insert(const CubeTreeUnit<T> &unit)
{
    if (!bb.contains(unit.getPosition()))
        return false;

    if(units.size() < capacity)
    {
        units.push_back(unit);
        return true;
    }

    if(!is_divided)
        subdivide();
    if(left_lower_bottom->insert(unit))
        return true;
    if(right_lower_bottom->insert(unit))
        return true;
    if(right_lower_front->insert(unit))
        return true;
    if(left_lower_front->insert(unit))
        return true;
    if(left_upper_bottom->insert(unit))
        return true;
    if(right_upper_bottom->insert(unit))
        return true;
    if(right_upper_front->insert(unit))
        return true;
    if(left_upper_front->insert(unit))
        return true;
    return false;
}

template<typename T>
void CubeTree<T>::getData(const BoundBox &volume, std::vector<CubeTreeUnit<T>> &data)
{
    if(!bb.intersects(volume))
    {
        return;
    }
    else
    {
        for(int i = 0; i < units.size(); i++)
        {
            if(volume.contains(units[i].getPosition()))
                data.push_back(units[i]);
        }

        if(is_divided)
        {
            left_lower_bottom->getData(volume, data)  ;
            right_lower_bottom->getData(volume, data);
            right_lower_front->getData(volume, data) ;
            left_lower_front->getData(volume, data);
            left_upper_bottom->getData(volume, data);
            right_upper_bottom->getData(volume, data);
            right_upper_front->getData(volume, data);
            left_upper_front->getData(volume, data);
        }
    }
}
#endif // OPENGLPROJECT_CUBETREE_H
