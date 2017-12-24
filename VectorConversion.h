struct FPolyVoxVector : public FVector
{
    FPolyVoxVector()
    {}
    
    explicit FPolyVoxVector(EForceInit E)
    : FVector(E)
    {}
    
    FPolyVoxVector(float InX, float InY, float InZ)
    : FVector(InX, InY, InX)
    {}
    
    FPolyVoxVector(const FVector &InVec)
    {
        FVector::operator=(InVec);
    }
    
    FPolyVoxVector(const PolyVox::Vector3DFloat &InVec)
    {
        FPolyVoxVector::operator=(InVec);
    }
    
    FVector& operator=(const PolyVox::Vector3DFloat& Other)
    {
        this->X = Other.getX();
        this->Y = Other.getY();
        this->Z = Other.getZ();
        
        DiagnosticCheckNaN();
        
        return *this;
    }
};
