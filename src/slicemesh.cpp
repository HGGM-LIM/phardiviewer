#include "slicemesh.h"

SliceMesh::SliceMesh()
{

}

surface SliceMesh::generateSliceMesh(std::__cxx11::string filename, int slice, Axis axis)
{
    surface sliceMesh;

    Image3DType::Pointer image = Image3DType::New();
    ReadImage< Image3DType >(filename, image);

    Image3DType::RegionType region = image->GetBufferedRegion();

    fmat rotMatrix;
    fmat spacingMatrix;
    fmat originMatrix;
    spacingMatrix.resize(1,3);
    originMatrix.resize(3,1);
    rotMatrix.resize(3,3);

    for (int i = 0; i < 3; i++)
    {
        originMatrix.at(i) = image->GetOrigin()[i];
        spacingMatrix.at(i) = image->GetSpacing()[i];

        for (int j = 0; j < 3; j++)
        {
            rotMatrix.at(i,j) = image->GetDirection()[i][j];
        }
    }

    spacingMatrix = diagmat(spacingMatrix);

    if (axis == Axis::X)
    {
        umat auxFaces1((region.GetSize()[1]-1)*(region.GetSize()[2]-1),3);
        umat auxFaces2((region.GetSize()[1]-1)*(region.GetSize()[2]-1),3);

        sliceMesh.vertices.resize(region.GetSize()[1]*region.GetSize()[2],3);
        fcolvec pixelValues(region.GetSize()[1]*region.GetSize()[2]);

        Index3DType coord_3d;
        coord_3d[0]=slice;

        int counter = 0;

        int ny = region.GetSize()[1];
        int nz = region.GetSize()[2];

        for (int z = 0; z < nz; ++z) {
            coord_3d[2]=z;

            if (z < nz - 1)
            {
                auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),0) = linspace<umat>(z*ny, (z+1)*ny - 2, ny - 1);
                auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),1) = auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),0) + 1;
                auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),2) = auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),1) + ny;

                auxFaces2(span(z*(ny-1),z*(ny-1) + ny -2),0) = auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),0);
                auxFaces2(span(z*(ny-1),z*(ny-1) + ny -2),1) = auxFaces1(span(z*(ny-1),z*(ny-1) + ny -2),2);
                auxFaces2(span(z*(ny-1),z*(ny-1) + ny -2),2) = auxFaces2(span(z*(ny-1),z*(ny-1) + ny -2),0) + ny;
            }

            for (int y = 0; y < ny; ++y) {
                coord_3d[1]=y;
                sliceMesh.vertices(counter,0) = slice;
                sliceMesh.vertices(counter,1) = y;
                sliceMesh.vertices(counter,2) = z;

                pixelValues(counter++) = image->GetPixel(coord_3d);
            }
        }
        sliceMesh.faces = join_vert(auxFaces1,auxFaces2);
        pixelValues = pixelValues + abs(min(pixelValues));
        pixelValues = pixelValues / max(pixelValues);
        sliceMesh.colors = repmat(pixelValues,1,3);

        sliceMesh.vertices = repmat(sliceMesh.vertices,2,1);
        sliceMesh.colors = repmat(sliceMesh.colors,2,1);
        sliceMesh.faces = join_vert(sliceMesh.faces,fliplr(sliceMesh.faces));
    }

    if (axis == Axis::Y)
    {
        umat auxFaces1((region.GetSize()[0]-1)*(region.GetSize()[2]-1),3);
        umat auxFaces2((region.GetSize()[0]-1)*(region.GetSize()[2]-1),3);

        sliceMesh.vertices.resize(region.GetSize()[0]*region.GetSize()[2],3);
        fcolvec pixelValues(region.GetSize()[0]*region.GetSize()[2]);

        Index3DType coord_3d;
        coord_3d[1]=slice;

        int counter = 0;

        int nx = region.GetSize()[0];
        int nz = region.GetSize()[2];

        for (int z = 0; z < nz; ++z) {
            coord_3d[2]=z;

            if (z < nz - 1)
            {
                auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),0) = linspace<umat>(z*nx, (z+1)*nx - 2, nx - 1);
                auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),1) = auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),0) + 1;
                auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),2) = auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),1) + nx;

                auxFaces2(span(z*(nx-1),z*(nx-1) + nx -2),0) = auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),0);
                auxFaces2(span(z*(nx-1),z*(nx-1) + nx -2),1) = auxFaces1(span(z*(nx-1),z*(nx-1) + nx -2),2);
                auxFaces2(span(z*(nx-1),z*(nx-1) + nx -2),2) = auxFaces2(span(z*(nx-1),z*(nx-1) + nx -2),0) + nx;
            }

            for (int x = 0; x < nx; ++x) {
                coord_3d[0]=x;
                sliceMesh.vertices(counter,0) = x;
                sliceMesh.vertices(counter,1) = slice;
                sliceMesh.vertices(counter,2) = z;

                pixelValues(counter++) = image->GetPixel(coord_3d);
            }
        }
        sliceMesh.faces = join_vert(auxFaces1,auxFaces2);
        pixelValues = pixelValues + abs(min(pixelValues));
        pixelValues = pixelValues / max(pixelValues);
        sliceMesh.colors = repmat(pixelValues,1,3);

        sliceMesh.vertices = repmat(sliceMesh.vertices,2,1);
        sliceMesh.colors = repmat(sliceMesh.colors,2,1);
        sliceMesh.faces = join_vert(sliceMesh.faces,fliplr(sliceMesh.faces));
    }

    if (axis == Axis::Z)
    {
        umat auxFaces1((region.GetSize()[0]-1)*(region.GetSize()[1]-1),3);
        umat auxFaces2((region.GetSize()[0]-1)*(region.GetSize()[1]-1),3);

        sliceMesh.vertices.resize(region.GetSize()[0]*region.GetSize()[1],3);
        fcolvec pixelValues(region.GetSize()[0]*region.GetSize()[1]);

        Index3DType coord_3d;
        coord_3d[2]=slice;

        int counter = 0;

        int nx = region.GetSize()[0];
        int ny = region.GetSize()[1];

        for (int y = 0; y < ny; ++y) {
            coord_3d[1]=y;

            if (y < ny - 1)
            {
                auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),0) = linspace<umat>(y*nx, (y+1)*nx - 2, nx - 1);
                auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),1) = auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),0) + 1;
                auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),2) = auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),1) + nx;

                auxFaces2(span(y*(nx-1),y*(nx-1) + nx -2),0) = auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),0);
                auxFaces2(span(y*(nx-1),y*(nx-1) + nx -2),1) = auxFaces1(span(y*(nx-1),y*(nx-1) + nx -2),2);
                auxFaces2(span(y*(nx-1),y*(nx-1) + nx -2),2) = auxFaces2(span(y*(nx-1),y*(nx-1) + nx -2),0) + nx;
            }

            for (int x = 0; x < nx; ++x) {
                coord_3d[0]=x;
                sliceMesh.vertices(counter,0) = x;
                sliceMesh.vertices(counter,1) = y;
                sliceMesh.vertices(counter,2) = slice;

                pixelValues(counter++) = image->GetPixel(coord_3d);
            }
        }
        sliceMesh.faces = join_vert(auxFaces1,auxFaces2);
        pixelValues = pixelValues + abs(min(pixelValues));
        pixelValues = pixelValues / max(pixelValues);
        sliceMesh.colors = repmat(pixelValues,1,3);

        sliceMesh.vertices = repmat(sliceMesh.vertices,2,1);
        sliceMesh.colors = repmat(sliceMesh.colors,2,1);
        sliceMesh.faces = join_vert(sliceMesh.faces,fliplr(sliceMesh.faces));
    }

    sliceMesh.vertices = rotMatrix * spacingMatrix * sliceMesh.vertices.t() + repmat(originMatrix,1,sliceMesh.vertices.n_rows);
    sliceMesh.vertices = sliceMesh.vertices.t();



    return sliceMesh;
}
