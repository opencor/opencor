The user-defined formats contained in this folder come from the CellML API (https://github.com/cellmlapi/cellml-api/). They should work fine in most cases, but be aware of the know limitations below.

MATLAB export:
 - If a model defines an ODE as a constant, e.g.
    dX/dt = 0
   then the exporter will export 0 as a constant of value 0 (!!). Then, due to 0- vs. 1-based indexing, that constant gets computed twice, if there are other constants in the model (!!). The end result is that dX/dt is not equal to 0, but to the value of the other constant.
