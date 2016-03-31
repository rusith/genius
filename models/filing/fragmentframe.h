#ifndef FRAGMENTFRAME_H
#define FRAGMENTFRAME_H
#include <QtCore>

/**
 * @brief use to manage files deferent fragments. this class contains the size of the fragment,end,start bytes
 */
class FragmentFrame
{
public:
  FragmentFrame();
  /**
   * @brief starting byte of the file fragment
   */
  quint64 start;
  /**
   * @brief size of the file fragment
   */
  quint64 size;

  /**
   * @brief operator =
   * @param rhs
   */
  void operator =(const FragmentFrame &rhs);

  /**
   * @brief equelity operator.
   * @param right hand side object
   * @return true if size and start is same else false
   */
  bool operator==(const FragmentFrame &other) const;

  /**
   * @brief operator <
   * @param rhs
   * @return
   */
  bool operator <(const FragmentFrame &rhs) const;

  /**
   * @brief operator >
   * @param other
   * @return
   */
  bool operator >(const FragmentFrame &other) const;

  /**
   * @brief operator <
   * @param other
   * @return
   */
  bool operator <(const quint64 &other)const;

  /**
   * @brief operator >
   * @param other
   * @return
   */
  bool operator >(const quint64 &other) const;

};

#endif // FRAGMENTFRAME_H
