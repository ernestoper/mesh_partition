#ifndef Elem_INC
#define Elem_INC

#include<vector>

#include"vec.h"
#include"SymMatrix.h"
#include "Grain.h"
#include "Node.h"


//#define BUILD_MESH_EDGE

//------------------------------------------------------
//   Topology declartion of geometrical element.
//   WW. 06.2005
//   WW. 02.2012
//------------------------------------------------------

namespace Mesh_Group
{
class Node;
class Edge;
class Mesh;
enum ElemType {line, quadri, hex, tri, tet, prism, pyramid};


//3.  Element declaration
class Elem:public Grain
{
   public:
      explicit Elem(const int Index) : Grain(Index), PatchIndex(0), quadratic(false)
      {}

      ~Elem();

      int getNodesNumber() const
      {
         switch(ele_Type)
         {
            case line:
               return 2;
            case quadri:
               return 4;
            case hex:
               return 8;
            case tri:
               return 3;
            case tet:
               return 4;
            case prism:
               return 6;
            case pyramid:
               return 5;
            default:
               break;
         }
         return 0;
      }

      int getNodesNumberHQ() const
      {
         switch(ele_Type)
         {
            case line:
               return 3;
            case quadri:
               return 9;
            case hex:
               return 20;
            case tri:
               return 6;
            case tet:
               return 10;
            case prism:
               return 15;
            case pyramid:
               return 14;
            default:
               break;
         }
         return 0;
      }

      int getNodesNumber(bool quad) const
      {
         if(quad) return getNodesNumberHQ();
         else return getNodesNumber();
      }

      int getEdgesNumber() const
      {
         switch(ele_Type)
         {
            case line:
               return 0;
            case quadri:
               return 4;
            case hex:
               return 12;
            case tri:
               return 3;
            case tet:
               return 6;
            case prism:
               return 9;
            case pyramid:
               return 8;
            default:
               break;
         }
         return 0;
      }

      int getFacesNumber() const
      {
         switch(ele_Type)
         {
            case line:
               return 2;
            case quadri:
               return 4;
            case hex:
               return 6;
            case tri:
               return 3;
            case tet:
               return 4;
            case prism:
               return 5;
            case pyramid:
               return 5;
            default:
               break;
         }
         return 0;
      }

      int Dim() const
      {
         switch(ele_Type)
         {
            case line:
               return 1;
            case quadri:
               return 2;
            case hex:
               return 3;
            case tri:
               return 2;
            case tet:
               return 3;
            case prism:
               return 3;
            case pyramid:
               return 3;
            default:
               break;
         }
         return 0;
      }

      int getElementType() const
      {
         return ele_Type;
      }

      int getPatchIndex() const
      {
         return PatchIndex;
      }

      std::string getName() const;

      // Nodes
      void getNodeIndeces(long  *node_index) const
      {
         for (int i=0; i< (int) nodes.Size(); i++)
            node_index[i]= static_cast<long>(nodes[i]->index);
      }
      size_t getNodeIndex(const int loc_lndex) const
      {
         return nodes[loc_lndex]->index;
      }
      void setNodes(Math_Group::vec<Node*>&  ele_nodes, const bool ReSize=false);
      void getNodes(Math_Group::vec<Node*>&  ele_nodes)
      {
         for (int i=0; i< (int) nodes.Size(); i++)
            ele_nodes[i]= nodes[i];
      }
      Node* getNode(const int i)
      {
         return nodes[i];
      }
      void MarkingNodes(bool maker);
      //
      void setLocalNodeIndex(const int li, const long n_lindex);
      //
      long getLocalNodeIndex(const int li) const;

#ifdef BUILD_MESH_EDGE
      // Edges
      void getEdges(Math_Group::vec<Edge*>&  ele_edges)
      {
         for (int i=0; i<nedges; i++) ele_edges[i]= edges[i];
      }
      Edge* getEdge(const int index)
      {
         return edges[index];
      }
      void setEdges(Math_Group::vec<Edge*>&  ele_edges)
      {
         for (int i=0; i<nedges; i++) edges[i]= ele_edges[i];
      }

      void setEdges_Orientation(Math_Group::vec<int>&  ori_edg)
      {
         for (int i=0; i<nedges; i++) edges_orientation[i]= ori_edg[i];
      }
#endif
      // Neighbors
      void setNeighbors(Math_Group::vec<Elem*>&  ele_neighbors)
      {
         for (int i=0; i<getFacesNumber(); i++)
            neighbors[i] = ele_neighbors[i];
      }
      void setNeighbor(const int LocalIndex, Elem* ele_neighbor)
      {
         neighbors[LocalIndex] = ele_neighbor;
      }
      void getNeighbors(Math_Group::vec<Elem*>&  ele_neighbors)
      {
         for (int i=0; i<getFacesNumber(); i++)
            ele_neighbors[i]= neighbors[i];
      }
      //Domain partition
      long getDomNodeIndex(const int loc_index)
      {
         return locnodes_index[loc_index];
      }
      void setDomNodeIndex(const int loc_index, const long dom_nindex)
      {
         locnodes_index[loc_index] = dom_nindex;
      }
      void AllocateLocalIndexVector()
      {
         locnodes_index.resize(nodes.Size());
      }
      void setDomainIndex(const int dom)
      {
         sub_dom = dom;
      }
      int getDomainIndex() const
      {
         return sub_dom;
      }
      // Local indicis
      void getLocalIndices_EdgeNodes(const int Edge, int *EdgeNodes);
      int getElementFaceNodes(const int Face, int *FacesNode);


      int getFaceType();

      void setOrder(const bool order)
      {
         quadratic = order;
      }

      // Output
      void Read(std::istream& is, Mesh_Group::Mesh *mesh, int fileType);
      void WriteIndex(std::ostream& os = std::cout) const;
      void WriteGmsh(std::ostream& os, const int sdom_idx = 0) const;
      void WriteGSmsh(std::ostream& os, bool quad = false) const;
      void WriteSubDOM(std::ostream& os, const long node_id_shift, bool quad = false) const;
      int getDataArray4BinaryOut(long *ivar, const long node_id_shift, bool quad = false) const;
      void WriteVTK_Type(std::ostream& os, bool isquad) const;
      void Write_index(std::ostream& os = std::cout) const;
      void WriteAll(std::ostream& os = std::cout) const;
      void WriteNeighbors(std::ostream& os = std::cout) const;
   private:
      // High order
      bool quadratic;

      int sub_dom;
      int no_faces_on_surface;
      //

      int PatchIndex;
      // Element type
      // 1 Line, 2 Quad, 3 Hex, 4 Tri, 5 Tet, 6 Pris
      ElemType ele_Type;
      //Math_Group::vec<long>   nodes_index;
      Math_Group::vec<long>   locnodes_index;
      Math_Group::vec<Node*>  nodes;

#ifdef BUILD_MESH_EDGE
      Math_Group::vec<Edge*>  edges;
      Math_Group::vec<int>  edges_orientation;
#endif
      Math_Group::vec<Elem*>  neighbors;
      //vec<Elem*>  sons;

      int nnodes_gl; //> number of ghost nodes for linear element
      std::vector<int>  ghost_nodes;

      // Private methods
      int getElementFaces1D(int *FaceNode);
      int getElementFacesTri(const int Face, int *FaceNode);
      int getElementFacesQuad(const int Face, int *FaceNode);
      int getElementFacesHex(const int Face, int *FaceNode);
      int getElementFacesTet(const int Face, int *FaceNode);
      int getElementFacesPri(const int Face, int *FaceNode);
      int getElementFacesPyramid(const int Face, int *FaceNode);
      friend class Mesh;

};

} //end namespace

#endif
