class Treatment < ActiveRecord::Base
  belongs_to :installation
  belongs_to :employee
end
