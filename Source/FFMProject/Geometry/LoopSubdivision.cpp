#include "FFMProject.h"
#include "LoopSubdivision.h"

namespace FFMGeometry
{

ModelPtr LoopSubdivision::Subdivide(ModelPtr pModel)
{
	ModelPtr pNextModel = ModelPtr(new Model);

	for (int i = 0, iFaceCount(pModel->GetFaceCount()); i < iFaceCount; ++i)
	{
		const HE_FacePtr& face = pModel->GetFace(i);

		FVector next_edge_positions[3];
		FVector next_vert_positions[3];

		// Calculates the next positions
		{
			int count = 0;
			HE_Edge* edge = face->edge_;
			do
			{
				if (edge->pair_)
				{
					next_edge_positions[count] =
						edge->vert_->pos_->p_ * 0.375f +
                        edge->next_->vert_->pos_->p_ * 0.375f +
						edge->prev_->vert_->pos_->p_ * 0.125f +
						edge->pair_->prev_->vert_->pos_->p_ * 0.125f;
				}
				else
				{
					next_edge_positions[count] =
						edge->vert_->pos_->p_ * 0.5f +
						edge->next_->vert_->pos_->p_ * 0.5f;
				}

				{
					TArray<HE_Vertex*> neighbors;
					edge->vert_->GetNeighboringVertices(neighbors);
					int n = neighbors.Num();
					float beta = n > 3 ? 3.0f / (8.0f*n) : 3.0f / 16.0f;
					next_vert_positions[count] = (1 - n*beta)* (edge->vert_->pos_->p_);
					for (int k = 0; k < n; ++k)
						next_vert_positions[count] += neighbors[k]->pos_->p_ * beta;
				}

				++count;
				edge = edge->next_;

			} while (edge != face->edge_ && count < 3);
		}

		// Makes next level faces based on new position calculated above
		{
            TArray<HE_Edge*> center_face_edges;
            TArray<FVector> next_face;
            next_face.Add(next_edge_positions[0]);
            next_face.Add(next_edge_positions[1]);
            next_face.Add(next_edge_positions[2]);
            pNextModel->AddFace(next_face, &center_face_edges);

			for (int k = 0; k < 3; ++k)
			{
				int prev_k = (k + 2) % 3;
				TArray<FVector> next_face;
				next_face.Add(next_vert_positions[k]);
				next_face.Add(next_edge_positions[k]);
				next_face.Add(next_edge_positions[prev_k]);
                
                TArray<HE_Edge*> face_edges;
				pNextModel->AddFace(next_face,&face_edges);
                
                face_edges[1]->SetPair(center_face_edges[(k+2)%3]);
            }
		}
	}

	pNextModel->SolveAllEdgePairs();
	return pNextModel;
}

void LoopSubdivision::Subdivide_2(ModelPtr pModel)
{
    ModelPtr pNextModel = ModelPtr(new Model);
    
    for( int i = 0, iCount(pModel->GetPosCount()); i < iCount; ++i )
    {
        const HE_Pos* p = pModel->GetPos(i);
        
        
    }
}
}