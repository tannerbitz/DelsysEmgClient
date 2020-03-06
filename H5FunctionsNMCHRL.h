


using namespace H5;

typedef struct TrialDatasets{
    DataSet JointAngle;      /* Nx7 element dataspace of type double */
    DataSet Force;           /* Nx2 element dataspace of type double */
    DataSet PerturbFlag;     /* Nx1 element dataspace of type int    */
    DataSet EndEffectorPO;   /* Nx6 element dataspace of type double */
};

H5File * CreateOrOpenFile(const std::string & filename){
    Exception::dontPrint();
    H5File * file = 0;

    try{
        /* Open file if it exists */
        file = new H5File(filename.c_str(), H5F_ACC_RDWR);
    }
    catch(const FileIException&){
        /* Create file if it does not exist */
        file = new H5File(filename.c_str(), H5F_ACC_TRUNC);
    }

    return file;
}

Group * CreateOrOpenGroup(H5File * file, H5std_string & group_name){

    Exception::dontPrint();
    Group * group = 0;

    try{
        /* Open group if it exists */
        group = new Group(file->openGroup(group_name));
    } catch(Exception & e){
        group = new Group(file->createGroup(group_name));
    }

    return group;
}

TrialDatasets CreateTrialDatasets(H5Location * h5loc){
    TrialDatasets td;

    // Define dataspace dimensions
    hsize_t dims_ja[2] = {10000, 7};
    hsize_t dims_force[2] = {10000, 2};
    hsize_t dims_pf[2] = {10000, 1};
    hsize_t dims_eepo[2] = {10000, 6};
    hsize_t maxdims_ja[2] = {H5S_UNLIMITED, 7};
    hsize_t maxdims_force[2] = {H5S_UNLIMITED, 2};
    hsize_t maxdims_pf[2] = {H5S_UNLIMITED, 1};
    hsize_t maxdims_eepo[2] = {H5S_UNLIMITED, 6};

    // Define dataspaces
    DataSpace dataspace_ja = DataSpace(2, dims_ja, maxdims_ja);
    DataSpace dataspace_force = DataSpace(2, dims_force, maxdims_force);
    DataSpace dataspace_pf = DataSpace(2, dims_pf, maxdims_pf);
    DataSpace dataspace_eepo = DataSpace(2, dims_eepo, maxdims_eepo);

    // Property List
    DSetCreatPropList prop_ja;
    DSetCreatPropList prop_force;
    DSetCreatPropList prop_pf;
    DSetCreatPropList prop_eepo;

    prop_ja.setChunk(2, dims_ja);
    prop_force.setChunk(2, dims_force);
    prop_pf.setChunk(2, dims_pf);
    prop_eepo.setChunk(2, dims_eepo);


    // Create or Open Datasets
    try{
        td.JointAngle = h5loc->openDataSet("JointAngle");
    }
    catch{
        td.JointAngle = h5loc->createDataSet("JointAngle",
        PredType::NATIVE_DOUBLE,
        dataspace_ja,
        prop_ja);
    }

    try{
        td.Force = h5loc->openDataSet("Force");
    }
    catch{
        td.Force = h5loc->createDataSet("Force",
        PredType::NATIVE_DOUBLE,
        dataspace_force,
        prop_force);
    }

    try{
        td.PerturbFlag = h5loc->openDataSet("PerturbFlag");
    }
    catch{
        td.PerturbFlag = h5loc->createDataSet("PerturbFlag",
        PredType::NATIVE_INT,
        dataspace_pf,
        prop_pf);
    }

    try{
        td.EndEffectorPO = h5loc->openDataSet("EndEffectorPO");
    }
    catch{
        td.EndEffectorPO = h5loc->createDataSet("EndEffectorPO",
        PredType::NATIVE_DOUBLE,
        dataspace_eepo,
        prop_eepo);
    }

    return td;
}

void CreateStringAttribute(H5Object * h5obj, H5std_string attr_name, H5std_string attr_val){
    // Create new dataspace for attribute -- H5S_SCALAR means the dataspace has 1 element
    DataSpace ds = DataSpace(H5S_SCALAR);

    // Create new string datatype for attribute
    StrType strdatatype(PredType::C_S1, 256); // of length 256 characters

    // Create attribute and write to it
    Attribute attr = h5obj->createAttribute(attr_name, strdatatype, ds);
    attr.write(strdatatype, attr_val);
}

void CreateNumericAttribute(H5Object * h5obj, H5std_string attr_name, double attr_val){
    // Create new dataspace for attribute -- H5S_SCALAR means the dataspace has 1 element
    DataSpace ds = DataSpace(H5S_SCALAR);

    // Create float datatype
    FloatType floatdatatype(PredType::NATIVE_DOUBLE);

    // Create attribute and write to it
    Attribute attr = h5obj->createAttribute(attr_name, floatdatatype, ds);
    attr.write(floatdatatype, &attr_val);
}
